#include "fileReader.h"


long systemMonitor::FileReader::findNumber(string sToFind, string fileToRead){
    ifstream info(fileToRead);
    string line;
    while(getline(info, line)){
       string sub = line.substr(0,sToFind.length());
       if(sub == sToFind){
         // find the beginning of the number
         string strToSearch = line.substr(0,100);
         int beginNum = -1;
         for(int i = 0; strToSearch[i] != '\0'; i++){
            int n = strToSearch[i] - 48;
            if(n >= 0 && n <= 9){
                 beginNum = i;
                 break;
            }
         }

          string number = line.substr(beginNum, 100); 
         // find the number
          int numberLen = 0;
          for(int i = number.length() - 1; i >= 0; i--){
              int num = number[i] - 48;
              if(num >= 0 && num <= 9){// the number has been reached
                  numberLen = i+1;
                  break;
              } 
          }
          long res = stol(number.substr(0, numberLen)); 
          return res;
       }
    }
   info.close();
   return 0;
}
void systemMonitor::FileReader::drawPlot(const std::vector<std::vector<int>>& plot){
    int prevPointY = 0;
    for(int i = plot.size()-1; i>=0; i--){ // draw the plot correctly
       if(i < 10){
         std::cout << " "; // for one-digit numbers
       }
       std::cout << i << " | ";
       for(int j = 0; j<plot[i].size(); j++){
           if(plot[i][j] == 1){
              std::cout << "  #  ";
           }
           else{
              std::cout << "  .  ";
           }
       }
        std::cout << std::endl;
    }
   std::cout << "--------------------------------------------";
   std::cout << "----------------------------------------------------------> t" << std::endl;
}
void systemMonitor::FileReader::visualizePlot(string sToFind, string fileToRead, long maxN){
    int maxY = 23, maxX = 20;

    std::vector<std::vector<int>> plot(maxY, std::vector<int>(maxX, 0));

    system("clear");
    for(int x = 0; x<maxX; x++){
 
       int y = 0;
       long n = this->findNumber(sToFind, fileToRead);
       if(n > maxY){
	   y = round((n * (maxY-1) * 1.0)/maxN);// proportion
       }
       else
         y = n;

        plot[y][x] = 1;
        this->drawPlot(plot);
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        system("clear");
    }
}
bool systemMonitor::FileReader::askForDouble(){
    cout << "Do you want to get the floating point result? (y/n)" << endl;
    char a;
    cin >> a;
    return a == 'y' ? true : false;
}

void systemMonitor::FileReader::countAverage(string strToFind, string fileToRead){
    this->timeCounter<uint64_t, uint64_t>(strToFind, fileToRead, [&](vector<uint64_t> v)->uint64_t {
                                                                    cout << "\nAverage: ";
                                                                    return this->findAverage<uint64_t, uint64_t>(v);});

    cout << "I am going to find the Geometric mean now..." << endl;
    this->timeCounter<cpp_dec_float_50, cpp_int>(strToFind, fileToRead, [&](vector<cpp_int> v)->cpp_dec_float_50 {
                                                               cout << "Geometric mean: " << endl;
                                                               return this->findGeoMean<cpp_int>(v);});

    bool wantDouble = this->askForDouble();
    if(wantDouble){
       this->timeCounter<double, uint64_t>(strToFind, fileToRead, [&](vector<uint64_t> v)->double {
                                                                  cout << "\nFloating point average: ";
                                                                  return this->findAverage<double, uint64_t>(v);});
    }else{
      return;
    }
}
void systemMonitor::FileReader::countDispersion(string strToFind, string fileToRead){
  // cpp_int from boost to store large results
   this->timeCounter<cpp_int, cpp_int>(strToFind, fileToRead, [&](vector<cpp_int> v)->cpp_int {
                                                                 cpp_int disp = this->findDispersion<cpp_int, cpp_int>(v);
                                                                 cpp_int sigma = sqrt(disp);
                                                                 cout << "\nStandard deviation: " << sigma << endl;
                                                                 cout << "Dispertion: ";
                                                                 return disp;
                                                               });
}
void systemMonitor::FileReader::countMedian(string strToFind, string fileToRead){
   this->timeCounter<long double, uint64_t>(strToFind, fileToRead, [&](vector<uint64_t> v)->long double {
                                                                       cout << "\nMedian: ";
                                                                       return this->findMedian(v);});
}
void systemMonitor::FileReader::countMode(string strToFind, string fileToRead){
   this->timeCounter<uint64_t, uint64_t>(strToFind, fileToRead, [&](vector<uint64_t> v)->uint64_t {
                                                                    cout << "\nMode: ";
                                                                    return this->findMode(v);});
}
void systemMonitor::FileReader::countMaxMin(string strToFind, string fileToRead){
   this->timeCounter<pair<uint64_t,uint64_t>, uint64_t>(strToFind, fileToRead,
                                                 [&](vector<uint64_t> v)->pair<uint64_t, uint64_t>{
                                                     pair<uint64_t, uint64_t> res = this->maxMin(v);
                                                     cout << "\nRange: " << res.first - res.second << endl;
                                                     cout << "Max" << "   " << "Min" << endl;
                                                     return res;
                                                 });
}
void systemMonitor::FileReader::countAsymmetryKurtosis(string strToFind, string fileToRead){
    this->timeCounter<pair<double,double>, cpp_int>(strToFind, fileToRead, [&](vector<cpp_int> v)->pair<double,double>{
                                                                            pair<double,double> res = this->
                                                                            asymmetryKurtosis<cpp_int, cpp_int>(v);
                                                                            cout << "\nAsymmetry and kurtosis: " << endl;
                                                                            return res;
                                                                            });
}

systemMonitor::Graph& systemMonitor::FileReader::buildCompleteGraph(const vector<uint64_t>& vals){
    // The initial vector is too large to build the complete graph
    // so I will take a sample of it
     vector<uint64_t> sample;

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, vals.size() - 1);
    for (int i = 0; i < 10000; ++i) {
        int index = dis(gen);
        sample.push_back(vals[index]);
    }

     int n = sample.size();
     Graph* g = new Graph();

     // create a vertex for each number
     for(int i = 0; i<n; ++i){
        boost::add_vertex(*g);
     }
    // Build the complete graph
    for(int i = 0; i < n; ++i) {
        for(int j = i + 1; j < n; ++j) {
            uint64_t weight = std::abs(static_cast<int64_t>(sample[i]) - static_cast<int64_t>(sample[j]));
            boost::add_edge(i, j, EdgeWeightProperty(weight), *g);
        }
    }
    cout << "There are " << boost::num_vertices(*g) << " vertices in the graph" << endl;
    return *g;
}
uint64_t systemMonitor::FileReader::findMST(const vector<uint64_t>& vals){
        Graph& g = this->buildCompleteGraph(vals);
        // Container to store the predecessor (parent) of each vertex in the MST
        vector<Vertex> predecessors(boost::num_vertices(g));

        // Run Prim's algorithm
        Vertex start = 0;  // Starting vertex for Prim's algorithm
        boost::prim_minimum_spanning_tree(g, &predecessors[0], boost::root_vertex(start));

       uint64_t total_cost = 0;
       for (size_t i = 0; i < predecessors.size(); ++i) {
            if (predecessors[i] != i) {  // The root vertex is its own predecessor
                Edge e;
                bool exists;
                boost::tie(e, exists) = boost::edge(predecessors[i], i, g);
                if (exists) {
                    uint64_t weight = boost::get(boost::edge_weight, g, e);
                    total_cost += weight;
                }
            }
       }
    // Clean up
    delete &g;
    return total_cost;
}
void systemMonitor::FileReader::countMST(string strToFind, string fileToRead){
    this->timeCounter<uint64_t, uint64_t>(strToFind, fileToRead, [&](vector<uint64_t> v)->uint64_t{
                                                                 uint64_t res = this->findMST(v);
                                                                 cout << "The minimum spanning tree of the graph, ";
                                                                 cout << "where vertices are the numbers and the edges' ";
                                                                 cout << "weights are the differences between them " << endl;
                                                                 return res;
                                                                });
}
