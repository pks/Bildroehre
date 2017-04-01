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
    ("f,file",            "file",                     cxxopts::value<string>())
    ("b,black_and_white", "image is black and white")
    ("h,hh",              "h",                        cxxopts::value<float>()->default_value("3.0"))
    ("C,hColor",          "hColor",                   cxxopts::value<float>()->default_value("3.0"))
    ("t,template_size",   "template window size",     cxxopts::value<int>()->default_value("7"))
    ("s,search_size",     "search window size",       cxxopts::value<int>()->default_value("21"))
    ("T,threads",         "number of threads",        cxxopts::value<int>()->default_value("1"))
    ("o,output",          "output file",              cxxopts::value<string>());

  opt.parse(argc, argv);

  auto& file = opt["f"].as<string>();
  auto is_bw = opt.count("b");
  auto& output = opt["o"].as<string>();
  float h = opt["h"].as<float>();
  float hColor = opt["C"].as<float>();
  float template_size = opt["t"].as<int>();
  float search_size = opt["s"].as<int>();
  setNumThreads(opt["T"].as<int>());

  Mat src = imread(file, 1);
  Mat dst;

  if (is_bw)
    fastNlMeansDenoising(src, dst, h, template_size, search_size);
  else
    fastNlMeansDenoisingColored(src, dst, h, hColor, template_size, search_size);

  imwrite(output, dst);

  return 0;
}

