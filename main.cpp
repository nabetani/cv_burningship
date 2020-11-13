#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <memory>
#include <omp.h>
#include <opencv2/opencv.hpp>

using namespace std;
constexpr double PI = 3.14159265358979323846;

struct ImageCalculator {

  uint32_t depth_;
  double cx_, cy_, w_;

  uint32_t calc(double cr, double ci) {
    double zr = 0;
    double zi = 0;
    for (uint32_t i = 0; i < depth_; ++i) {
      double azr = abs(zr);
      double azi = abs(zi);
      zr = zr * zr - zi * zi + cr;
      zi = 2 * azi * azr + ci;
      double d2 = zr * zr + zi * zi;
      if (4 < d2) {
        return static_cast<uint32_t>((1u << 16) + i * 128 - sqrt(d2) * 32);
      }
    }
    return ~0u;
  }
  double yval(double imy) const { //
    return (imy - 0.5) * w_ + cy_;
  }
  double xval(double imx) const { //
    return (imx - 0.5) * w_ + cx_;
  }
};

struct Rgb {
  uint8_t r_, g_, b_;
};

struct ColorMap {
  enum { size = 1 << 10 };
  enum { mask = size - 1 };
  unique_ptr<Rgb[]> m_;
  ColorMap()
      : m_(new Rgb[size]) //
  {
    init();
  }
  void init() {
    for (int ix = 0; ix < size; ++ix) {
      m_[ix] = colorAt(ix);
    }
  }
  constexpr double f_inv(double f) {
    const double delta = 6.0 / 29;
    return delta < f       //
               ? f * f * f //
               : (f - 128.0 / 116) * 3 * delta * delta;
  };
  double nonlinear(double x0) {
    auto x = std::clamp<double>(x0, 0, 1);
    return x < 0.0031308   //
               ? x * 12.92 //
               : 1.055 * std::pow(x, 1 / 2.4) - 0.055;
  };

  Rgb colorAt(int ix) {
    constexpr double yn = 0.578;
    constexpr double xn = 0.549;
    constexpr double zn = 0.629;
    constexpr double AB = 100;
    const double th = ix * (2 * PI / size);
    const double a = cos(th) * AB;
    const double b = sin(th) * AB;
    constexpr double L = 80;
    constexpr double fy = (L + 16) / 116;
    const double fx = fy + a / 500;
    const double fz = fy - b / 200;
    const double y = yn * f_inv(fy);
    const double x = xn * f_inv(fx);
    const double z = zn * f_inv(fz);
    const double rr = nonlinear(3.24 * x - 1.54 * y - 0.499 * z);
    const double rg = nonlinear(-0.969 * x + 1.88 * y + 0.0415 * z);
    const double rb = nonlinear(0.0557 * x - 0.204 * y + 1.057 * z);
    const double m = 255.4; // / Math.max(rr, rg, rb);
    return {cv::saturate_cast<uint8_t>(rr * m),
            cv::saturate_cast<uint8_t>(rg * m),
            cv::saturate_cast<uint8_t>(rb * m)};
  }
};

cv::Mat createImage(uint32_t wpix, uint32_t d, double cx, double cy, double w) {
  cv::Mat im = cv::Mat::zeros(wpix, wpix, CV_8UC3);
  ColorMap cm;
  ImageCalculator ic{d, cx, cy, w};
  double const wpixInv = 1.0 / wpix;
#pragma omp parallel for num_threads(64) schedule(dynamic)
  for (uint32_t iy = 0; iy < wpix; ++iy) {
    auto pix = im.ptr(iy, 0);
    double y = ic.yval(iy * wpixInv);
    for (uint32_t ix = 0; ix < wpix; ++ix, pix += 3) {
      auto c = ic.calc(ic.xval(ix * wpixInv), y);
      if (~c) {
        auto const &col = cm.m_[c & cm.mask];
        pix[0] = col.r_;
        pix[1] = col.g_;
        pix[2] = col.b_;
      } else {
        pix[0] = 0;
        pix[1] = 0;
        pix[2] = 0;
      }
    }
  }
  return im;
}

int main(int argc, char const *argv[]) {
  uint32_t wpix = argc < 2 ? 800 : atoi(argv[1]);
  uint32_t d = argc < 3 ? 200 : atoi(argv[2]);
  double cx = argc < 4 ? -0.5 : atof(argv[3]);
  double cy = argc < 5 ? -0.5 : atof(argv[4]);
  double w = argc < 6 ? 3.5 : atof(argv[5]);
  std::string fn = argc < 7 ? "hoge.png" : argv[6];
  std::cout << "wpix=" << wpix                    //
            << "  d=" << d                        //
            << "  c=(" << cx << ", " << cy << ")" //
            << "  w=" << w                        //
            << "  fn=" << fn                      //
            << std::endl;
  auto im = createImage(wpix, d, cx, cy, w);
  cv::imwrite(fn, im);
  return 0;
}