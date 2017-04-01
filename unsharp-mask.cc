#include <iostream>
#include <vector>
#include "opencv2/opencv.hpp"
#include "opencv2/photo/photo.hpp"
#include "cxxopts/src/cxxopts.hpp"

using namespace std;
using namespace cv;

int
main (int argc, char** argv)
{
  cxxopts::Options opt(argv[0], " - Options");
  opt.add_options()
    ("f,file",   "file",                                cxxopts::value<string>())
    ("s,sigma",  "sigma for Gaussian Blur",             cxxopts::value<float>()->default_value("3.0"))
    ("a,alpha",  "weight of the first array elements",  cxxopts::value<float>()->default_value("1.5"))
    ("b,beta",   "weight of the second array elements", cxxopts::value<float>()->default_value("-0.5"))
    ("g,gamma",  "scalar added to each sum",            cxxopts::value<float>()->default_value("0"))
    ("o,output", "output file",                         cxxopts::value<string>());

  opt.parse(argc, argv);

  auto& file = opt["f"].as<string>();
  auto& output = opt["o"].as<string>();
  float sigma = opt["s"].as<float>();
  float alpha = opt["a"].as<float>();
  float beta  = opt["b"].as<float>();

  Mat src = imread(file, 1), dst;
  cv::GaussianBlur(src, dst, cv::Size(0, 0), sigma);
  cv::addWeighted(src, alpha, dst, beta, 0, dst);
  imwrite(output, dst);

  return 0;
}

