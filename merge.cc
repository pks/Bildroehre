#include <iostream>
#include <vector>
#include "opencv2/opencv.hpp"
#include "opencv2/photo/photo.hpp"
#include "cxxopts/src/cxxopts.hpp"

using namespace std;
using namespace cv;

int
load_images(const vector<string> &files, vector<Mat> &images)
{
  for (const auto& f : files) {
    cerr << "reading " << f << endl;
    images.push_back(imread(f, 1));
    if (images.back().empty()) {
      cerr << "failed to read " << f << endl;
      return 1;
    }
  }

  return 0;
}

void
mertens_fusion(vector<Mat> &images, string output)
{
  cerr << "Mertens fusion" << endl;
  Ptr<MergeMertens> merge_mertens = createMergeMertens();
  Mat m;
  merge_mertens->process(images, m);

  cerr << "writing " << output << endl;
  imwrite(output, m * 255);
}

void
hdr(vector<Mat> &images, string output, string algorithm, string tonemap,
    vector<float> times, float gamma)
{
  cerr << "HDR (" << algorithm << "/" << tonemap << ")" << endl;

  Mat response, hdr, ldr;
  Ptr<CalibrateCRF> calibrate;
  Ptr<MergeExposures> merge;
  if (algorithm == "Debevec") {
    calibrate = createCalibrateDebevec();
    merge = createMergeDebevec();
  }
  else
  if (algorithm == "Robertson") {
    calibrate = createCalibrateRobertson();
    merge = createMergeRobertson();
  }
  calibrate->process(images, response, times);
  merge->process(images, hdr, times, response);

  Ptr<Tonemap> map;
  if (tonemap == "Durand") {
    map = createTonemapDurand(gamma);
  } else
  if (tonemap == "Drago") {
    map = createTonemapDrago(gamma);
  } else
  if (tonemap == "Reinhard") {
    map = createTonemapReinhard(gamma);
  } else
  if (tonemap == "Mantiuk") {
    map = createTonemapMantiuk(gamma);
  }
  map->process(hdr, ldr);

  cerr << "writing " << output << endl;
  imwrite(output, ldr * 255);
}


int
main (int argc, char** argv)
{
  cxxopts::Options opt(argv[0], " - Options");
  opt.add_options()
    ("f,files",     "files",                   cxxopts::value<vector<string>>())
    ("F,Mertens",   "Mertens fusion")
    ("D,debevec",   "Debevec HDR")
    ("R,robertson", "Robertson HDR")
    ("w,times",     "times for HDR",           cxxopts::value<vector<float>>())
    ("t,tonemap",   "tonemapping",             cxxopts::value<string>())
    ("g,gamma",     "gamma for HDR",           cxxopts::value<float>())
    ("o,output",    "output file",             cxxopts::value<string>())
    ("O,Output",    "another output file",     cxxopts::value<string>())
    ("p,OUTPUT",    "another output file",     cxxopts::value<string>());

  opt.parse(argc, argv);

  auto& files = opt["f"].as<vector<string>>();
  vector<Mat> images;
  auto do_fusion = opt.count("F");
  bool do_debevec_hdr = opt.count("D");
  bool do_robertson_hdr = opt.count("R");
  bool do_hdr = do_debevec_hdr||do_robertson_hdr;
  vector<float> times = opt["w"].as<vector<float>>();
  float gamma = opt["g"].as<float>();
  cout << gamma << endl;
  if (do_hdr && !opt.count("g"))
    gamma = 1.0;
  string tonemap;
  if (opt.count("t"))
    tonemap = opt["t"].as<string>();
  else
    tonemap = "Durand";
  auto& output = opt["o"].as<string>();
  auto& output1 = opt["O"].as<string>();
  auto& output2 = opt["p"].as<string>();

  if (load_images(files, images) > 0)
    return 1;

  if (do_fusion)
    mertens_fusion(images, output);

  if (do_hdr) {
    if (do_debevec_hdr)
      hdr(images, output1, "Debevec", tonemap, times, gamma);
    if (do_robertson_hdr)
      hdr(images, output2, "Robertson", tonemap, times, gamma);
  }

  return 0;
}

