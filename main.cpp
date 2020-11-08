#include <cmath>
#include <iostream>
#include <omp.h>
#include <opencv2/opencv.hpp>

int main(int argc, char const *argv[]) {
  unsigned long long volatile x = 0;
  int const N = 1000000;
#pragma omp parallel for
  for (int i = 0; i < N; ++i) {
    x += std::sin(++argc) * 77 == 1 ? 0 : 1;
  }
  cv::Mat m = cv::Mat::zeros(1, 1, CV_8UC1);
  std::cout << "m:" << m << "\n";
  std::cout << "x:" << (x * 100.0 / N) << "%" << std::endl;
}