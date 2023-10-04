#include <iostream>
#include <opencv4/opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int getCenterLine(Mat &curr_frame, bool draw_line = true) {
    Mat color_frame, canny_frame;
    vector<cv::Vec4i> lines;
    inRange(curr_frame, Scalar(0, 20, 0), Scalar(255, 255, 255), color_frame);
    Canny(color_frame, canny_frame, 50, 150, 3);
    HoughLinesP(canny_frame, lines, 1, CV_PI / 180, 100, 200, 10);

    if (!lines.empty()) {
        // 选取两条最长的直线
        vector<cv::Vec4i> longestLines;
        sort(lines.begin(), lines.end(), [](const cv::Vec4i &a, const cv::Vec4i &b) {
            return cv::norm(cv::Point(a[0], a[1]) - cv::Point(a[2], a[3])) >
                   cv::norm(cv::Point(b[0], b[1]) - cv::Point(b[2], b[3]));
        });
        longestLines.emplace_back(lines[0]);
        longestLines.emplace_back(lines[1]);

        // 计算中线位置
        cv::Point middlePoint;
        middlePoint.x = (longestLines[0][2] + longestLines[1][2]) / 2;
        middlePoint.y = (longestLines[0][3] + longestLines[1][3]) / 2;

        if (draw_line) {
            // 在原始图像上绘制中线
            cv::line(curr_frame, cv::Point(longestLines[0][0], longestLines[0][1]),
                     cv::Point(longestLines[0][2], longestLines[0][3]), cv::Scalar(0, 0, 255), 2);
            cv::line(curr_frame, cv::Point(longestLines[1][0], longestLines[1][1]),
                     cv::Point(longestLines[1][2], longestLines[1][3]), cv::Scalar(0, 0, 255), 2);
            cv::circle(curr_frame, middlePoint, 5, cv::Scalar(0, 255, 0), -1);
        }

        return middlePoint.x;
    }
    return 200;
}

int main() {
    // read raw_frame
    Mat image = imread("./turn1.jpg");
    // resize raw_frame
    resize(image, image, Size(400, 300));
    imwrite("./resize.jpg", image);
    // blur raw_frame
    Mat blur_image;
    medianBlur(image, blur_image, 5);
    // binary raw_frame
    Mat binary_image;
    inRange(blur_image, Scalar(0, 150, 0), Scalar(255, 255, 255), binary_image);
    imwrite("binary.jpg", binary_image);

    Mat canny_image, center_image;
    Canny(binary_image, canny_image, 50, 150, 3);
    imwrite("canny.jpg", canny_image);

    // copy raw_frame
    center_image = binary_image.clone();
    for (int i = 0; i < center_image.rows; i++) {
        int l, r;
        for (l = 1; l < center_image.cols - 1; l++) {
            int c = center_image.at<uchar>(i, l) +
                    center_image.at<uchar>(i, l + 1) +
                    center_image.at<uchar>(i, l - 1);
            if (c == 255 * 3)
                break;
        }
        for (r = center_image.cols - 2; r >= 1; r--) {
            int c = center_image.at<uchar>(i, r) +
                    center_image.at<uchar>(i, r + 1) +
                    center_image.at<uchar>(i, r - 1);
            if (c == 255 * 3)
                break;
        }

        int center = (l + r) >> 1;
        center_image.at<uchar>(i, center) = 0;
    }
    imwrite("center.jpg", center_image);

    //    vector<cv::Vec4i> lines;
    //    HoughLinesP(canny_image, lines, 1, CV_PI / 180, 100, 20, 1000);
    //    for (auto &line: lines) {
    //        cv::line(center_image, cv::Point(line[0], line[1]), cv::Point(line[2], line[3]), cv::Scalar(120, 255, 255), 1);
    //    }
    //    imwrite("center.jpg", center_image);
}