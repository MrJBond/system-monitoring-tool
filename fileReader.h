#include <iostream>
#include <fstream>
#include <cmath>
#include <random>
#include <vector>
#include <thread>
#include <chrono>
#include <mutex>
#include <future>
#include <condition_variable>
#include <functional>
#include <numeric>
#include <algorithm>
#include <memory>
#include <unordered_map>
#include <climits>
#include <unistd.h> // for sysconf
#include <fcntl.h> // for file control
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/multiprecision/cpp_dec_float.hpp>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/properties.hpp>
#include <boost/graph/named_function_params.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>


using namespace std;
using namespace boost::multiprecision; // for large numbers
namespace systemMonitor{


     // To print a pair
     template<typename T, typename U>
     ostream& operator<<(ostream& os, std::pair<T, U> p){
         os << p.first << " " << p.second;
         return os;
      }

         // Graph
     typedef boost::property<boost::edge_weight_t, uint64_t> EdgeWeightProperty;
     typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
                                   boost::no_property,
                                   EdgeWeightProperty> Graph;
     typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
     typedef boost::graph_traits<Graph>::edge_descriptor Edge;




    struct FileReader{

        virtual void read() = 0;
        void countAverage(string strToFind, string fileToRead);
        void countDispersion(string strToFind, string fileToRead);
        void countMedian(string strToFind, string fileToRead);
        void countMode(string strToFind, string fileToRead);
        void countMaxMin(string strToFind, string fileToRead);
        void countAsymmetryKurtosis(string strToFind, string fileToRead);
        void countMST(string strToFind, string fileToRead);
        // another params...

        void visualizePlot(string sToFind, string fileToRead, long maxN);

        protected:
            // Works any for the file, where there is only one number in the line
            long findNumber(string sToFind, string fileToRead);// get the number by the name of string
             void drawPlot(const std::vector<std::vector<int>>& plot);

             template<typename T, typename V> // 2 params because we may wanna return, for example, double for vector of ints
                T findAverage(const vector<V>& vals){

                   if(vals.size() == 0){
                       return T(0);
                   }

                    T sum = accumulate(vals.begin(), vals.end(), V(0));
                    return sum / vals.size();
             }
             template<typename T, typename V>
             T findDispersion(const vector<V>& vals){

                 if(vals.size() == 0){
                     return T(0);
                 }
                 vector<V> squared;
                 for_each(vals.begin(), vals.end(), [&](V n){
                     squared.push_back(n*n);
                 });

                 T ave2 = this->findAverage<T,V>(squared);
                 T ave = this->findAverage<T,V>(vals);

                 return (ave2 - ave*ave);
             }
             long double findMedian(vector<uint64_t> vals){ // pass by value because we need to sort it

                 if(vals.size() == 0){
                    return 0.0;
                 }
                 sort(vals.begin(), vals.end());
                 uint64_t size = vals.size();
                 long double m = 0;
                 if(size % 2 != 0){
                      m = vals[size/2];
                 }else{
                      m = (vals[size/2]+vals[size/2-1])/2.0;
                 }
                 return m;
             }
             uint64_t findMode(const vector<uint64_t>& vals){

                if(vals.size() == 0){
                    return 0;
                }
                unordered_map<uint64_t, int> table;
                for(const auto& e : vals){
                    table[e]++;
                }
                uint64_t mode = 0;
                int max = INT_MIN;
                for(const auto& p : table){
                    if(max < p.second){
                        max = p.second;
                        mode = p.first;
                    }
                }
                return mode;
             }
             pair<uint64_t, uint64_t> maxMin(const vector<uint64_t>& vals){

                if(vals.size() == 0){
                    return make_pair(0,0);
                }
                uint64_t max = *max_element(vals.begin(), vals.end());
                uint64_t min = *min_element(vals.begin(), vals.end());
                return make_pair(max, min);
             }

             // using pointers to solve the issue with the inheritace of non-copiable types
             shared_ptr<mutex> mx = make_shared<mutex>();
             shared_ptr<condition_variable> cv = make_shared<condition_variable>();
             shared_ptr<atomic_bool> ready = make_shared<atomic_bool>(false);
             template<typename T, typename V>
             pair<double,double> asymmetryKurtosis(const vector<V>& vals){

                 if(vals.size() == 0){
                     return make_pair(0.0, 0.0);
                 }
                 // As = Mu3/(sigma^3)
                 // Ex = Mu4/(sigma^4) - 3
                 // Mu3 = V3 - 3*V1V2 + 2V1^3
                 // Mu4 = V4 - 4V1V3 + 6V1^2*V2 - 3V1^4
                 // V1 = M[X], V2 = M[X^2], V3 = M[X^3], V4 = M[X^4]

                 vector<V> sq, cub, pow4;

                  auto computeSquares = async(launch::async, [&](){

                    // ensure that only the thread is writing to these vectors
                     vector<V> local_sq, local_cub, local_pow4;
                     for_each(vals.begin(), vals.end(), [&](V n){
			lock_guard<mutex> guard(*mx);

                        local_sq.push_back(n*n);
                        local_cub.push_back(n*n*n);
                        local_pow4.push_back(pow(n,4));
                     });
                    // transfer ownership of the data from the local vectors to the member vectors
                     lock_guard<mutex> guard(*mx);
                     sq = move(local_sq);
                     cub = move(local_cub);
                     pow4 = move(local_pow4);

                     // signal the threads to start working
                    *ready = true;
                    cv->notify_all();
                  });

                  /*T v1 = this->findAverage<T,V>(vals);
                  T v2 = this->findAverage<T, V>(sq);
                  T v3 = this->findAverage<T, V>(cub);
                  T v4 = this->findAverage<T, V>(pow4);
                  T sigma = sqrt(this->findDispersion<T, V>(vals));*/

                 // Using threads to count faster

                  auto f1 = async(launch::async, [&]()-> T {
                             // waiting for the signal from the loop's thread
                             unique_lock<mutex> lock(*mx);
                             cv->wait(lock,[&]{return ready->load();});
                             return this->findAverage<T,V>(vals);
                  });
                  auto f2 = async(launch::async, [&]()-> T {
                             // waiting for the signal from the loop's thread
                             unique_lock<mutex> lock(*mx);
                             cv->wait(lock,[&]{return ready->load();});
                             return this->findAverage<T,V>(sq);
                  });
                  auto f3 = async(launch::async, [&]()-> T {
                          // waiting for the signal from the loop's thread
                          unique_lock<mutex> lock(*mx);
                          cv->wait(lock,[&]{return ready->load();});
                          return this->findAverage<T,V>(cub);
                  });
                  auto f4 = async(launch::async, [&]()-> T {
                            // waiting for the signal from the loop's thread
                             unique_lock<mutex> lock(*mx);
                             cv->wait(lock,[&]{return ready->load();});
                            return this->findAverage<T,V>(pow4);
                  });
                  auto s = async(launch::async, [&]()-> T {
                             // waiting for the signal from the loop's thread
                             unique_lock<mutex> lock(*mx);
                             cv->wait(lock,[&]{return ready->load();});
                             return sqrt(this->findDispersion<T,V>(vals));
                  });
                  T v1 = f1.get();
                  T v2 = f2.get();
                  T v3 = f3.get();
                  T v4 = f4.get();
                  T sigma = s.get();

                 T Mu3 = v3 - 3*v1*v2 + 2*v1*v1*v1;
                 T Mu4 = v4 - 4*v1*v3 + 6*v1*v1*v2 - 3*pow(v1,4);

                 double As = 0.0;
                 double kur = 0.0;
                 try{
                     if(sigma == 0){
                         throw runtime_error("Dispersion is 0!");
                     }
                     As =  static_cast<double>(Mu3)/(static_cast<double>(sigma*sigma*sigma));
                     kur = static_cast<double>(Mu4)/static_cast<double>(pow(sigma,4)) - 3;
                 }catch(const runtime_error& e){
                     cout << "\nAn error has occurred: " << e.what() << endl;
                 }
                 return make_pair(As,kur);
             }
             template<typename V>
             cpp_dec_float_50 findGeoMean(const vector<V>& vals){ // using large float type
                 size_t n = vals.size();
                 if(n == 0){
                     return 0;
                  }

                  V mult = accumulate(vals.begin(), vals.end(), V(1), [](V a, V b)-> V {return a*b;});
                  cpp_dec_float_50 r = static_cast<cpp_dec_float_50>(mult);
                  return pow(r, 1.0/n);
             }
             uint64_t findMST(const vector<uint64_t>& vals);
             // another params...


             template<typename T, typename V>
             void timeCounter(string strToFind, string fileToRead, function<T(vector<V>)> f){

                  int second;
                  while(1){
                     cout << "How long do you want to count? (from 1 to 120)" << endl;
                     cin >> second;
                     if(second > 120 || second < 1){
                        cout << "Incorrect number of seconds" << endl;
                     }
                     else{
                        system("clear");
                        break;
                     }
                 }
                   // visualize counting
                  this->stopCount = false;
                  thread counting([&](){this->countVisualizer();});

                  auto start = chrono::steady_clock::now();
                  vector<V> vals;
                  while(1){
                      auto end = chrono::steady_clock::now();
                      if(chrono::duration_cast<chrono::seconds>(end - start).count() > second){
                          this->stopCount = true;
                          counting.join();
                          break;
                      }
                      long num = this->findNumber(strToFind, fileToRead);
                      V n = static_cast<V>(num);
                      vals.push_back(n);
                 }
                 T res =  f(vals);
                 cout << '\n' << res << endl;
                 this_thread::sleep_for(chrono::seconds(5));
             }
    private:
       bool stopCount;
       void countVisualizer(){

           auto startCount = chrono::steady_clock::now();

           vector<char> statusBar;
           auto printStatus = [&](){
               statusBar.push_back('#');
                for(const auto& c : statusBar){
                    cout << c;
                }
                cout << endl;
           };

           while(!stopCount){
               if(stopCount){
                  break;
               }
               auto endCount = chrono::steady_clock::now();
               auto n = chrono::duration_cast<chrono::seconds>(endCount - startCount).count();
               system("clear");
               cout << "Counting... " << n << " s" << endl << endl;
               printStatus();
               cout.flush();
               this_thread::sleep_for(chrono::milliseconds(1000));
           }
     }
     bool askForDouble();

     // Graph
     Graph& buildCompleteGraph(const vector<uint64_t>& vals);
};

}
