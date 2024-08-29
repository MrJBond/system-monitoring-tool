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

string systemMonitor::FileReader::getFileName(string sToFind){
     // Find time now
    auto curTime = chrono::system_clock::to_time_t(chrono::system_clock::now());
    string timeNow = ctime(&curTime);

   // Remove the newline character at the end of timeNow
   timeNow.erase(timeNow.find_last_not_of(" \n\r\t") + 1);

   string name = sToFind + "_" +  timeNow + ".txt";
   return name;
}
void systemMonitor::FileReader::askUserForFile(function<void()> f){
    char a;
    cout << "Do you want to write this result to a file? (y/n)" << endl;
    cin >> a;
    if(a == 'y'){
      f();
    }
    else{
      return;
    }
}
void systemMonitor::FileReader::plotToFile(string sToFind, const vector<vector<int>>& plot){

    string name = this->getFileName(sToFind);

   // Write to file
   ofstream file(name);

   this->drawPlot<ofstream&>(file,plot);

   file.close();
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
        this->drawPlot<ostream&>(std::cout, plot);

        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        system("clear");
    }
    // Ask for a file
   this->askUserForFile([&](){this->plotToFile(sToFind, plot);});
}
bool systemMonitor::FileReader::askForDouble(){
    cout << "Do you want to get the floating point result? (y/n)" << endl;
    char a;
    cin >> a;
    return a == 'y' ? true : false;
}

void systemMonitor::FileReader::countAverage(string strToFind, string fileToRead){
    this->timeCounter<pair<string, uint64_t>, uint64_t>(strToFind, fileToRead, [&](vector<uint64_t> v)->pair<string,uint64_t> {
                                                                    string title =  "Average: ";
                                                                    uint64_t a = this->findAverage<uint64_t, uint64_t>(v);
                                                                    auto res = make_pair(title, a);
                                                                    return res;
							          });

    cout << "I am going to find the Geometric mean now..." << endl;
    this->timeCounter<pair<string, cpp_dec_float_50>, cpp_int>(strToFind, fileToRead, [&](vector<cpp_int> v)->auto {
                                                               // return type of this lambda is pair<string, cpp_dec_float_50>
                                                               string title = "Geometric mean: ";
                                                               cpp_dec_float_50 gm = this->findGeoMean<cpp_int>(v);
                                                               auto res = make_pair(title, gm);
                                                               return res;
                                                               });

    bool wantDouble = this->askForDouble();
    if(wantDouble){
       this->timeCounter<pair<string, double>, uint64_t>(strToFind, fileToRead, [&](vector<uint64_t> v)->pair<string,double> {
                                                                  string title = "Floating point average: ";
                                                                  double a = this->findAverage<double, uint64_t>(v);
                                                                  return make_pair(title, a);
                                                                  });
    }else{
      return;
    }
}
void systemMonitor::FileReader::countDispersion(string strToFind, string fileToRead){
  // cpp_int from boost to store large results
   this->timeCounter<tuple<string, cpp_int, string, cpp_int>, cpp_int>(strToFind, fileToRead, [&](vector<cpp_int> v)->auto {
                                                                 cpp_int disp = this->findDispersion<cpp_int, cpp_int>(v);
                                                                 cpp_int sigma = sqrt(disp);
                                                                 string title1 =  "Standard deviation: ";
                                                                 string title2 = "\nDispertion: ";
                                                                 return make_tuple(title1, sigma, title2, disp);
                                                               });
}
void systemMonitor::FileReader::countMedian(string strToFind, string fileToRead){
   this->timeCounter<pair<string, long double>, uint64_t>(strToFind, fileToRead, [&](vector<uint64_t> v)->auto {
                                                                       string title = "Median: ";
                                                                       long double m = this->findMedian(v);
                                                                       return make_pair(title, m);
                                                                      });
}
void systemMonitor::FileReader::countMode(string strToFind, string fileToRead){
   this->timeCounter<pair<string, uint64_t>, uint64_t>(strToFind, fileToRead, [&](vector<uint64_t> v)->pair<string,uint64_t> {
                                                                    string title = "Mode: ";
                                                                    uint64_t m = this->findMode(v);
                                                                    return make_pair(title, m);
                                                                    });
}
void systemMonitor::FileReader::countMaxMin(string strToFind, string fileToRead){
   this->timeCounter<tuple<string,uint64_t,string,uint64_t,string, uint64_t>, uint64_t>(strToFind, fileToRead,
                                                 [&](vector<uint64_t> v)->auto{
                                                     pair<uint64_t, uint64_t> r = this->maxMin(v);
                                                     string title1 = "Range: ";
                                                     uint64_t range = r.first - r.second;
                                                     auto res = make_tuple(title1, range, "\nMax", r.first, "Min", r.second);
                                                     return res;
                                                 });
}
void systemMonitor::FileReader::countAsymmetryKurtosis(string strToFind, string fileToRead){
    this->timeCounter<tuple<string,double,double>, cpp_int>(strToFind, fileToRead, [&](vector<cpp_int> v)->tuple<string,double,double>{
                                                                            pair<double,double> res = this->
                                                                            asymmetryKurtosis<cpp_int, cpp_int>(v);
                                                                            string title =  "Asymmetry and kurtosis: ";
                                                                            return make_tuple(title, res.first, res.second);
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
    this->timeCounter<pair<string, uint64_t>, uint64_t>(strToFind, fileToRead,[&](vector<uint64_t> v)->pair<string, uint64_t>{
                                                                 uint64_t res = this->findMST(v);
                                                                 string title =  "The minimum spanning tree of the graph, "
                                                                  "where vertices are the numbers and the edges' "
                                                                 "weights are the differences between them \n";
                                                                 return make_pair(title, res);
                                                                });
}

cpp_int systemMonitor::FileReader::findMaxST(const vector<uint64_t>& vals){
    Graph& g = this->buildCompleteGraph(vals);

    // Convert the weights to 1/weight
     boost::property_map<Graph, boost::edge_weight_t>::type weightmap = get(boost::edge_weight, g);
    auto edges_pair = boost::edges(g);
    for (auto it = edges_pair.first; it != edges_pair.second; ++it) {
       weightmap[*it] = 1.0 / weightmap[*it];  // Invert the weight
    }
   // Find the MinST
   vector<Vertex> predecessors(boost::num_vertices(g));
   Vertex start = 0;
   boost::prim_minimum_spanning_tree(g, &predecessors[0], boost::root_vertex(start));

   // Get the MaxST cost
   cpp_int total_cost = 0;
    for (size_t i = 0; i < predecessors.size(); ++i) {
        if (predecessors[i] != i) {  // The root vertex is its own predecessor
            Edge e;
            bool exists;
            boost::tie(e, exists) = boost::edge(predecessors[i], i, g);
            if (exists) {
                total_cost += static_cast<cpp_int>(1.0 / weightmap[e]);  // Use the original weight before inversion
            }
        }
    }
    delete &g;
    return total_cost;
}
void systemMonitor::FileReader::countMaxST(string strToFind, string fileToRead){
    this->timeCounter<pair<string, cpp_int>, uint64_t>(strToFind, fileToRead, [&](vector<uint64_t> v)->pair<string,cpp_int>{
                                                                cpp_int res = this->findMaxST(v);
                                                                string title =  "The maximum spanning tree of the graph, "
                                                                "where vertices are the numbers and the edges' "
                                                                "weights are the differences between them \n";
                                                                return make_pair(title,res);
                                                                });
}
