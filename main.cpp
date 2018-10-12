#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>
#include <vector>
#include <dirent.h>
#include <opencv2/opencv.hpp>


int readFile(cv::Mat &img, const std::string path, const std::string ptsname)
{
    float x,y;
    std::string path_temp = path;
    std::ifstream in;
    bool start_flag = false;
    int point_index = 0;
    path_temp = path_temp + ptsname;
    path_temp = path_temp + ".pts";
//    std::cout << path_temp << std::endl;
    in.open(path_temp, std::ios::in);
    if (!in.is_open()){
        std::cout << "open File Failed." << std::endl;
        return -1;
    }
    std::string strOne;
    while (getline(in, strOne)){
//        std::cout << strOne << std::endl;
        if(strOne=="{\r" || strOne=="{")
        {
            start_flag = true;
//            std::cout << "========start read data========" << std::endl;
            continue;
        }
        if(strOne=="}\r" || strOne=="}")
        {
//            std::cout << "======== end read data ========" << std::endl;
            return -2;
        }
        if(start_flag)
        {
            std::stringstream ss;
            ss << strOne;
            ss >> x >> y;
            cv::circle(img, cv::Point(x, y), 2, cv::Scalar(0, 0, 255), -1);
//            std::cout << "point_index:" << point_index << ": x=" << x << "; y=" << y << std::endl;
            point_index++;
        }
    }
    return 0;
}


void int2str(const int &int_temp,std::string &string_temp)
{
    std::stringstream stream;
    stream<<int_temp;
    string_temp=stream.str();   //此处也可以用 stream>>string_temp
}


int main() {

    std::string path = "/media/luckynote/Data/face-recognition/face-eval/300VW_Dataset_2015_12_14/";
    DIR *pDir;
    struct dirent* ptr;
    std::string pathVid;
    std::string pathVidName;

    std::string ptsname;

    cv::VideoCapture camera;
    cv::Mat frame;

    if(!(pDir = opendir(path.c_str()))) return -1;
    while((ptr = readdir(pDir))!=0)
    {
        if (strcmp(ptr->d_name, ".") != 0 && strcmp(ptr->d_name, "..") != 0)
        {
            pathVid = path + ptr->d_name;
            pathVidName = pathVid + "/vid.avi";
//            std::cout << pathVidName << std::endl;
            camera.open(pathVidName);

            int pts_index = 0;

            while(true)
            {
                camera >> frame;
                if(frame.empty())
                {
                    camera.release();
                    break;
                }
                pts_index++;

                int2str(pts_index,ptsname);
                int ptsnamelength = ptsname.length();
                for(int k=6-ptsnamelength;k>0;k--)
                {
                    ptsname = "0" + ptsname;
                }
                int flag = readFile(frame, pathVid + "/annot/", ptsname);

                cv::imshow("frame", frame);

                if('q' == cv::waitKey(25))
                {
                    camera.release();
                    break;
                }

                if(flag==-1)
                {
                    break;
                }else if(flag == -2);
                {
                    continue;
                }
            }

        }
    }
    closedir(pDir);
    return 0;
}