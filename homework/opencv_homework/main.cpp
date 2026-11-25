#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>

using namespace cv;
using namespace std;


void detectArmor(Mat &src, Mat &out)
{
    out = src.clone();
    Mat gray, binary;

    //转灰度
    cvtColor(src, gray, COLOR_BGR2GRAY);

    //二值化
    threshold(gray, binary, 120, 255, THRESH_BINARY);

    //查找外轮廓
    vector<vector<Point>> contours;
    findContours(binary, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);

    vector<RotatedRect> light_rects;

    for (auto &cnt : contours)
    {
        
        double area = contourArea(cnt);
        if (area < 30)
            continue;

        RotatedRect rrect = minAreaRect(cnt);
        Size2f s = rrect.size;
        float h = max(s.height, s.width);
        float w = min(s.height, s.width);
        float ratio = h / w;

        
        if (ratio > 2.0 && ratio < 8.0)
        {
            light_rects.push_back(rrect);
        }
    }

    
    for (int i = 0; i < (int)light_rects.size(); i++)
    {
        for (int j = i + 1; j < (int)light_rects.size(); j++)
        {
            RotatedRect l1 = light_rects[i];
            RotatedRect l2 = light_rects[j];

            
            float h1 = max(l1.size.height, l1.size.width);
            float h2 = max(l2.size.height, l2.size.width);
            float dh = fabs(h1 - h2);
            float dangle = fabs(l1.angle - l2.angle);

            if (dh < h1*0.4 && dangle < 15)
            {
                
                Point2f pts1[4], pts2[4];
                l1.points(pts1);
                l2.points(pts2);

                
                for (int k = 0; k < 4; k++)
                {
                    line(out, pts1[k], pts1[(k + 1) % 4], Scalar(0, 0, 255), 2);
                    line(out, pts2[k], pts2[(k + 1) % 4], Scalar(0, 0, 255), 2);
                    circle(out, pts1[k], 3, Scalar(0, 255, 0), -1); //四角绿色圆点标记
                    circle(out, pts2[k], 3, Scalar(0, 255, 0), -1);
                }

                
                vector<Point2f> armor_pts;
                armor_pts.push_back(pts1[0]);
                armor_pts.push_back(pts1[1]);
                armor_pts.push_back(pts2[2]);
                armor_pts.push_back(pts2[3]);

                
                Point2f center(0,0);
                for(auto p : armor_pts){
                    center.x += p.x;
                    center.y += p.y;
                }
                center.x /= 4.0f;
                center.y /= 4.0f;

                
                circle(out, center, 5, Scalar(255, 0, 0), -1);
            }
        }
    }
}


void fftSpectrumDemo()
{
    Mat img = imread("data/04_phoenix_fft.png", IMREAD_GRAYSCALE);
    if(img.empty()){
        cout << "无法读取fft图片 data/04_phoenix_fft.png" << endl;
        return;
    }
    Mat padded;
    int m = getOptimalDFTSize(img.rows);
    int n = getOptimalDFTSize(img.cols);
    copyMakeBorder(img, padded, 0, m-img.rows, 0, n-img.cols, BORDER_CONSTANT, Scalar::all(0));

    Mat planes[] = {Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F)};
    Mat complexI;
    merge(planes,2,complexI);
    dft(complexI, complexI);

    split(complexI, planes);
    magnitude(planes[0], planes[1], planes[0]);
    Mat magI = planes[0];

    magI += Scalar::all(1);
    log(magI, magI);

    magI = magI(Rect(0,0, magI.cols & -2, magI.rows & -2));
    int cx = magI.cols / 2;
    int cy = magI.rows / 2;

    Mat q0(magI, Rect(0,0,cx,cy));
    Mat q1(magI, Rect(cx,0,cx,cy));
    Mat q2(magI, Rect(0,cy,cx,cy));
    Mat q3(magI, Rect(cx,cy,cx,cy));

    Mat tmp;
    q0.copyTo(tmp); q3.copyTo(q0); tmp.copyTo(q3);
    q1.copyTo(tmp); q2.copyTo(q1); tmp.copyTo(q2);

    normalize(magI, magI, 0,1,NORM_MINMAX);
    imshow("原图", img);
    imshow("FFT频谱（看隐藏文字）", magI);
    imwrite("out_spectrum.png", magI*255);
}

int main()
{
    
    vector<string> img_paths = {
        "data/1.jpg",
        "data/2.jpg",
        "data/3.jpg"
    };
    for(auto &path : img_paths)
    {
        Mat src = imread(path);
        if(src.empty())
        {
            cout << "读取失败：" << path << endl;
            continue;
        }
        Mat res;
        detectArmor(src, res);

        imshow(path, res);
        string out_name = "out_" + path.substr(5);
        imwrite(out_name, res);
        cout << "保存图片结果 -> " << out_name << endl;
    }

    
    VideoCapture cap("data/avi.mp4");
    if(!cap.isOpened())
    {
        cerr << "打开视频失败 data/avi.mp4" << endl;
    }
    else
    {
        double fps = cap.get(CAP_PROP_FPS);
        int w = cap.get(CAP_PROP_FRAME_WIDTH);
        int h = cap.get(CAP_PROP_FRAME_HEIGHT);
VideoWriter writer("out_armor.avi", VideoWriter::fourcc('M','J','P','G'), fps, Size(w,h));
         Mat frame, frame_out;
         while(cap.read(frame))
         {
             detectArmor(frame, frame_out);
             writer.write(frame_out);
             imshow("video", frame_out);
             if(waitKey(1) == 27) break; //ESC退出
         }
         writer.release();
         cap.release();
         cout << "视频输出完成 out_armor.avi" << endl;
     }
     
     fftSpectrumDemo();
     cout << "\n全部任务执行完毕，按任意键关闭窗口" << endl;
     waitKey(0);
     destroyAllWindows();
     return 0;
 }
 