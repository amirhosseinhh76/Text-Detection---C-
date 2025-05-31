#include <opencv2/opencv.hpp>
#include <iostream>
void detectText(const std::string& inputPath) {

    cv::Mat image = cv::imread(inputPath);
    if (image.empty())
    {
        std::cout << "Error: Could not read the image." << std::endl;
        return;
    }

    /// grayscale
    cv::Mat gray;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

    /// adaptive thresholding
    cv::Mat thresh;
    cv::adaptiveThreshold(gray, thresh,255,cv::ADAPTIVE_THRESH_MEAN_C,cv::THRESH_BINARY_INV,3,35);

    ///Morphological closing
    cv::Mat closed;
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT,cv::Size(3, 3));
    cv::morphologyEx(thresh, closed, cv::MORPH_CLOSE, kernel,cv::Point(-1,-1),1);

    /// contour
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(closed,contours,cv::RETR_EXTERNAL,cv::CHAIN_APPROX_SIMPLE);

    for (const auto& contour : contours) {
        cv::Rect bbox = cv::boundingRect(contour);

        if (bbox.width > 20 && bbox.height > 10) {
            int pad = 5;  /// box padding
            bbox.x = std::max(0, bbox.x - pad);
            bbox.y = std::max(0, bbox.y - pad);
            bbox.width = std::min(image.cols - bbox.x, bbox.width + 2*pad);
            bbox.height = std::min(image.rows - bbox.y, bbox.height + 2*pad);

            cv::rectangle(image, bbox, cv::Scalar(0, 255, 0), 2);
        }
    }

    cv::imshow("Detected Text", image);
    cv::waitKey(0);
}

int main() {
    std::string imagePath = "sign.jpg";
    detectText(imagePath);
    return 0;
}
