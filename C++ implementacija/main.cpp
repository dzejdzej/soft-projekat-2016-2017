#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>

#include <stdio.h>
#include <iostream>
#include <sstream>

#include "hough.h"
using namespace std;
using namespace cv;

Mat img_edge;
Mat img_blur;
Mat img_ori;
Mat img_ori1;

int threshold_value = 0;
int threshold_type = 3;;
int const max_value = 255;
int const max_type = 4;
int const max_BINARY_value = 255;
Mat* img_ORI;
/// Global Variables
bool use_mask;
Mat img; Mat templ; Mat mask; Mat result;
const char* image_window = "Source Image";
const char* result_window = "Result window";

int match_method;
int max_Trackbar = 5;
int j = 0;

const char* CW_IMG_ORIGINAL 	= "Result";
const char* CW_IMG_EDGE		= "Canny Edge Detection";
const char* CW_ACCUMULATOR  	= "Accumulator";

/// Function Headers
void MatchingMethod( int, void*, string, string );

void doTransform(Mat, int, string, Point&);


/**
 * @function main
 */
int main( int argc, char** argv )
{
  templ = imread("delic.png");

  /// Load image and template
  for(int i = 0; i < 10; i++) {
      //char* intStr = itoa(i);
     // string str = string(intStr);
      stringstream ss;
      ss << i;
      string myString = ss.str();

        stringstream ss1;
      ss1 << j;
      string myString1 = ss1.str();


      string path = "images\\frame-" + myString + ".png";

      img = imread(path, CV_LOAD_IMAGE_COLOR );

      int w = img_edge.cols;
      int h = img_edge.rows;


      if(img.empty() || templ.empty() || (use_mask && mask.empty()))
      {
        cout << "Can't read one of the images" << endl;
        return -1;
      }

      /// Create windows
      //namedWindow( image_window, WINDOW_AUTOSIZE );
      //namedWindow( result_window, WINDOW_AUTOSIZE );
      MatchingMethod( 0, 0, path, myString1);
      waitKey(0);
  }



  return 0;
}

/**
 * @function MatchingMethod
 * @brief Trackbar callback
 */
void MatchingMethod( int, void*, string path, string myString1)
{
  /// Source image to display
  Mat img_display;
  img.copyTo( img_display );

  /// Create the result matrix
  int result_cols =  img.cols - templ.cols + 1;
  int result_rows = img.rows - templ.rows + 1;

  result.create( result_rows, result_cols, CV_32FC1 );

  /// Do the Matching and Normalize
  bool method_accepts_mask = (CV_TM_SQDIFF == match_method || match_method == CV_TM_CCORR_NORMED);
  if (use_mask && method_accepts_mask)
    { matchTemplate( img, templ, result, match_method); }
  else
    { matchTemplate( img, templ, result, match_method); }

  normalize( result, result, 0, 1, NORM_MINMAX, -1, Mat() );

  /// Localizing the best match with minMaxLoc
  double minVal; double maxVal; Point minLoc; Point maxLoc;
  Point matchLoc;

  //minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );
//cout << minVal << endl;
/////
 minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());
    cout << "min: " << minVal << " at " << minLoc << endl;
    cout << "max: " << maxVal << " at " << maxLoc << endl;
    cout << result.at<float>(minLoc.y, maxLoc.x) << endl;
    //////
    float rezultat = result.at<float>(minLoc.y, maxLoc.x);
  if(rezultat > 0.89) {

        /// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
      if( match_method  == TM_SQDIFF || match_method == TM_SQDIFF_NORMED )
        { matchLoc = minLoc; }
      else
        { matchLoc = maxLoc; }

        cout << "**********************" << endl;
        cout << matchLoc << endl;
        cout << "**********************" << endl;
/*
        Mat drawing1 = Mat::zeros( Size(img_display.cols, img_display.rows), CV_8UC1 );
        Mat drawing2 = Mat::ones( Size(img_display.cols, img_display.rows), CV_8UC1 );


        //Mat drawing2 = img_display;
        drawing1(Range(466, drawing1.rows),Range(0,drawing1.cols))=1;
        cout<< drawing1 << endl;
        cout << "/////////////////////" << endl;
cout<< drawing2 << endl;

        imshow("drawing1",drawing1);
        imshow("drawing2",drawing2);

        Mat res;
        bitwise_and(drawing1,drawing2, res);
        imshow("AND",res);
*/

        Mat drawing1 = Mat::zeros( Size(img_display.cols,img_display.rows), CV_8UC1 );
        Mat drawing2 = Mat::zeros( Size(img_display.cols,img_display.rows), CV_8UC1 );

        drawing1(Range(minLoc.y+120,drawing1.rows),Range(0,drawing1.cols))=255;
        //imshow("drawing1",drawing1);
        Mat src_gray;

        //cvtColor(img_display, src_gray, CV_BGR2GRAY);
        //threshold(src_gray, drawing2, 150, 255, 1);

        drawing2 = img_display;
        img_display.convertTo(drawing2, CV_8U);
        imshow("drawing2",drawing1);
        Mat res;
        bitwise_and( img_display, img_display, res, drawing1 );
//imshow("fa", res);
        //bitwise_and(drawing2, drawing1, res);
        //imshow("AND",res);
        //imwrite( "Res_Image.jpg", res );
    //"D:\\c++2016\\TestYela\\Res_img.jpg"
        doTransform(res, 100, myString1, minLoc);

        /// Show me what you got
        rectangle( img_display, matchLoc, Point( matchLoc.x + templ.cols , matchLoc.y + templ.rows ), Scalar::all(0), 2, 8, 0 );
        rectangle( result, matchLoc, Point( matchLoc.x + templ.cols , matchLoc.y + templ.rows ), Scalar::all(0), 2, 8, 0 );

  }
      //imshow( image_window, img_display );
      //imshow( result_window, result );
return;
}


void doTransform(Mat res, int threshold, string myString1, Point& minLoc)
{
	Mat img_edge;
	Mat img_blur;

    Mat img = res;
    //namedWindow( "Original image", CV_WINDOW_AUTOSIZE );
    //imshow( "Original image", img );
    Mat gray, edge, draw;
    blur( img, img, Size(5,5) );

    cvtColor(img, gray, CV_BGR2GRAY);

    Canny( gray, edge, 50, 150, 3);

    edge.convertTo(draw, CV_8U);
    //namedWindow("image", CV_WINDOW_AUTOSIZE);
    //imshow("image", draw);

	int w = edge.cols;
	int h = edge.rows;
	//Transform
	keymolen::Hough hough;
	hough.Transform(edge.data, w, h);

	if(threshold == 0)
		threshold = w>h?w/4:h/4;

	while(1)
	{
		Mat img_res = img.clone();

		//Search the accumulator
		vector< pair< pair<int, int>, pair<int, int> > > lines = hough.GetLines(threshold);

		//Draw the results
		vector< pair< pair<int, int>, pair<int, int> > >::iterator it;
		for(it=lines.begin();it!=lines.end();it++)
		{
			line(img_res, Point(it->first.first, it->first.second), Point(it->second.first, it->second.second), Scalar( 0, 0, 255), 5, 8);
		}

		//Visualize all
		int aw, ah, maxa;
		aw = ah = maxa = 0;
		const unsigned int* accu = hough.GetAccu(&aw, &ah);

		for(int p=0;p<(ah*aw);p++)
		{
			if((int)accu[p] > maxa)
				maxa = accu[p];
		}
		double contrast = 1.0;
		double coef = 255.0 / (double)maxa * contrast;

		Mat img_accu(ah, aw, CV_8UC3);
		for(int p=0;p<(ah*aw);p++)
		{
			unsigned char c = (double)accu[p] * coef < 255.0 ? (double)accu[p] * coef : 255.0;
			img_accu.data[(p*3)+0] = 255;
			img_accu.data[(p*3)+1] = 255-c;
			img_accu.data[(p*3)+2] = 255-c;
		}


		//imshow(CW_IMG_ORIGINAL, img_res);
		//imshow(CW_IMG_EDGE, edge);
		//imshow(CW_ACCUMULATOR, img_accu);

		char c = waitKey(360000);
		if(c == '+')
			threshold += 5;
		if(c == '-')
			threshold -= 5;
		if(c == 27)
			break;

        //inRange(img_res, Scalar(0, 0, 255), Scalar(0, 0, 255), img_res);
        //imshow("ndfkajf", img_res);

/*
SimpleBlobDetector::Params params;
params.minDistBetweenBlobs = 2.0;  // minimum 10 pixels between blobs
params.filterByArea = true;         // filter my blobs by area of blob
params.minArea = 20.0;              // min 20 pixels squared
params.maxArea = 500.0;             // max 500 pixels squared
SimpleBlobDetector myBlobDetector(params);
vector<KeyPoint> myBlobs;
myBlobDetector.detect(img_res, myBlobs);

Mat blobImg;
drawKeypoints(img_res, myBlobs, blobImg);

for(vector<KeyPoint>::iterator blobIterator = myBlobs.begin(); blobIterator != myBlobs.end(); blobIterator++){
   cout << "size of blob is: " << blobIterator->size << endl;
   cout << "point is at: " << blobIterator->pt.x << " " << blobIterator->pt.y << std::endl;

   line(img_res, Point(blobIterator->pt.x-50, blobIterator->pt.y-25), Point(blobIterator->pt.x + 50, blobIterator->pt.y+15), Scalar( 0, 0, 255), 5, 8);
   imshow("Blobs", img_res);

}

*/

//line(img_res, Point(minLoc.x-150, minLoc.y+150), Point(minLoc.x + 200 ,minLoc.y +150 ), Scalar( 0, 255, 0), 5, 8);
   //imshow("Blobs", img_res);

        string path = "results\\frame-" + myString1 + ".png";
//imshow("fds", img_res);
        imwrite(path, img_res);
        j++;
        break;

	}
}
