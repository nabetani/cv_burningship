#include <cmath>
#include <cstdint>
#include <iostream>

#include <omp.h>
#include <opencv2/opencv.hpp>

using namespace std;

int main(int argc, char const *argv[]) {
  uint32_t w = argc < 2 ? 800 : atoi(argv[1]);
  uint32_t d = argc < 3 ? 800 : atoi(argv[2]);
  std::string fn = argc < 4 ? "hoge.png" : argv[3];
  std::cout << "w=" << w     //
            << "  d=" << d   //
            << "  fn=" << fn //
            << std::endl;
  auto im = cv::Mat::zeros(w, w, CV_8UC1);
  cv::imwrite(fn, im);
  return 0;
}