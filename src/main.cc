#include <iostream>
#include <opencv2/opencv.hpp>
#include <tools/tools.h>
using namespace cv;
int main(int argc, char** argv)
{
  if (argc != 3)  
  {
    std::cerr << "./Usg: ./demo image_path img_count" << std::endl;
    return 1;
  } 
  std::vector<std::pair<std::string, std::string>> v_stereo_imgs;
  int img_count = tools::load_image(argv[1], std::stoi(argv[2]), v_stereo_imgs);

  for (int i = 0; i < v_stereo_imgs.size(); i++)
  {
    std::cout << "{" 
                << v_stereo_imgs[i].first << "\n" 
                << v_stereo_imgs[i].second 
	      << "}"
	      << std::endl;
    const std::pair<std::string, std::string>& stereo_img = v_stereo_imgs[i];
    cv::Mat left_img = cv::imread(stereo_img.first, 0);
    cv::Mat right_img = cv::imread(stereo_img.second, 0);
    std::vector<cv::Point2f> left_corners;
    std::vector<cv::Point2f> right_corners;

    bool left_flag = cv::findChessboardCorners(left_img, cv::Size(9, 6), left_corners, CALIB_CB_ADAPTIVE_THRESH | CALIB_CB_NORMALIZE_IMAGE); 
    bool right_flag = cv::findChessboardCorners(right_img, cv::Size(9, 6), right_corners, CALIB_CB_ADAPTIVE_THRESH | CALIB_CB_NORMALIZE_IMAGE); 

    if (left_flag && right_flag)
    {
#if 0
      cv::cornerSubPix(left_img, left_corners, Size(11,11), Size(-1,-1),
                               TermCriteria(TermCriteria::COUNT+TermCriteria::EPS,
			                                             30, 0.01));
      cv::cornerSubPix(right_img, right_corners, Size(11,11), Size(-1,-1),
                               TermCriteria(TermCriteria::COUNT+TermCriteria::EPS,			                                             30, 0.01));
#endif
      cv::Mat mask, R, t;
      cv::Mat E = cv::findEssentialMat(left_corners, right_corners, 1.0, cv::Point2d(0, 0), RANSAC, 0.999, 1.0, mask);
      cv::recoverPose(E, left_corners, right_corners, cv::Mat::eye(3, 3, CV_64F), R, t, mask);
      cv::Mat R_vec;
      cv::Rodrigues(R, R_vec);
      std::cout << "{" << i +1 << std::endl;
      std::cout << "||R_vec||: " << cv::norm(R_vec) * 57.296 << std::endl;
      std::cout << "||t||: " << cv::norm(t) << std::endl;;
      std::cout << "t: " << t.t() << std::endl;
      
      for (int j = 0; j < left_corners.size(); j++)
      {
        cv::Mat p3f_l = (cv::Mat_<double>(3, 1) << left_corners[j].x, left_corners[j].y, 1);
	cv::Mat p3f_r = (cv::Mat_<double>(3, 1) << right_corners[j].x, right_corners[j].y, 1);
        cv::Mat error = p3f_r.t() * E * p3f_l;
	std::cout << error << std::endl;
        cv::circle(left_img, left_corners[j], 2, cv::Scalar(100), 2);
	cv::putText(left_img, std::to_string(j), left_corners[j], 1, 1, cv::Scalar::all(80));
        cv::circle(right_img, right_corners[j], 2, cv::Scalar(100), 2);
	cv::putText(right_img, std::to_string(j), right_corners[j], 1, 1, cv::Scalar::all(80));
      }
      std::cout << "}" << std::endl;

      cv::imshow("left", left_img);
      cv::imshow("right", right_img);
      cv::waitKey(0);
      std::cout << "show img " << i + 1 << std::endl;
    }

  }
 
  return 0;
}
