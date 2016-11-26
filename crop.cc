#include <iostream>
#include <vector>

#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

Mat src, img, roi;
Rect crop_rect(0,0,0,0);
vector<cv::Point> pt;
const char* win_name="crop";

void
show_img ()
{
  img = src.clone();
  if(crop_rect.width > 0 && crop_rect.height > 0){
    roi = src(crop_rect);
    cout << crop_rect.width << " " << crop_rect.height << endl;
    imshow("cropped", roi);
  }

  //rectangle(img, crop_rect, Scalar(0,255,0), 1, 8, 0 );
  imshow(win_name,img);
}

void
mouse_handler (int event, int x, int y, int f, void*)
{
  switch (event)
  {
    case CV_EVENT_LBUTTONDOWN:
      circle(src, Point(x,y), 4, 0);
      pt.push_back(cv::Point(x,y));
      show_img();
      break;
  }

  if (pt.size() == 2) {
    //circle(src, Point(pt[1].x, pt[0].y), 4, 0);
    //circle(src, Point(pt[0].x, pt[1].y), 4, 0);
    crop_rect = Rect(pt[0], pt[1]);
    show_img();
  }
}

int
main (int argc, char** argv)
{
  namedWindow(win_name, WINDOW_NORMAL);
  namedWindow("cropped", WINDOW_NORMAL);
  setMouseCallback(win_name, mouse_handler, NULL);

  for (size_t i=1; i<argc; i++) {
    string imgn(argv[i]);
    src=imread(argv[i],1);
    //destroyWindow(win_name);
    destroyWindow("cropped");
    crop_rect.x = 0;
    crop_rect.y = 0;
    crop_rect.height = 0;
    crop_rect.width = 0;
    pt.clear();

    imshow(win_name,src);

    while(1){
      char c=waitKey();
      if(c == 's' && roi.data){
        imshow(win_name, src);
        roi=src(crop_rect);
        imwrite(imgn+"-crop.jpg", roi);
        cout << " saved " << imgn <<endl;
        break;
      }

      if(c==27) break;

      show_img();
    }
  }

  return 0;
}

