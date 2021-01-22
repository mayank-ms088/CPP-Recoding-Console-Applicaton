#include "opencv2/opencv.hpp"
#include <iostream>
#include <ctime>
#include <cstdio>
#include <time.h>
#include <stdio.h>
using namespace cv;
using namespace std;
vector<string> ls; /// list of videos
int c = 0,flag=0;
string pa;

map<string, int> data_username;
map<string, string> Password;

void gausian_blur(Mat& image, Mat& frame_res) {
    GaussianBlur(image, frame_res, Size(7, 7), 1);
}


void video_rec() {
    //////////////////// Added Part
    time_t start, end;
    //////////////////// Added Part
    VideoCapture vcap(0, CAP_DSHOW);
    if (!vcap.isOpened()) {
        cout << "Error opening video stream or file" << endl;
        return;
    }
    vcap.set(CAP_PROP_FRAME_WIDTH, 640);
    vcap.set(CAP_PROP_FRAME_WIDTH, 480);
    //int frame_width = vcap.get(CAP_PROP_FRAME_WIDTH);
    //int frame_height = vcap.get(CAP_PROP_FRAME_HEIGHT);


    string file_name = "record_";
    //cout << "Name Of Video-" << endl;
   // cin >> file_name;
    char video_count = char('0' + c);
    file_name += video_count;
    file_name += ".avi";
    string s1=pa;
    s1 += "/";
    file_name =  pa + file_name;


    VideoWriter video(file_name, VideoWriter::fourcc('M', 'J', 'P', 'G'), 10, Size(640, 480), false);
    ls.push_back(file_name);


    //starting time
    time(&start);

    map<int, int> M;
    for (;;) {

        Mat frame;
        vcap >> frame;

        char c = (char)waitKey(33);
        if (c == 27) break;
        Mat fr = frame;
        gausian_blur(frame, fr);
        //seperating channels 
        vector<Mat> rgb;
        split(fr, rgb);
        // 2 is the red channel


      // frame = frame_res;

        video.write(rgb[2]);
        imshow("Frame", rgb[2]);

        //Current Time
        time(&end);
        double dif = difftime(end, start);
        M[dif]++;
        if (M[dif] == 1)cout << "time--> " << dif << "s" << endl;

        if (dif == 10)
        {
            cout << "DONE!!!" << endl;
            vcap.release();
            video.release();
            destroyAllWindows();
            break;
        }
    }
}


bool login() {

    if (flag == 0) {
        cout << "You are a New User, Please Set Your User Name and Password." << endl;
        string usr, paswd;
        cout << "Set your UserName-";
        cin >> usr;
        cout << "Set your Password-";
        cin >> paswd;
        flag++;
        data_username[usr]++;
        Password[usr] = paswd;
    }
    string userName;
    string userPassword;
    int loginAttempt = 0, f = 0;
    while (loginAttempt < 5)
    {
        cout << "Please enter your user name: ";
        cin >> userName;
        cout << "Please enter your user password: ";
        cin >> userPassword;

        if (Password[userName] == userPassword)
        {
            cout << userName << "!\n"; f = 1;
            break;
        }
        else
        {
            cout << "Invalid login attempt. Please try again.\n" << '\n';
            loginAttempt++;
        }
    }
    if (loginAttempt == 5)
    {
        cout << "Too many login attempts! The program will now terminate.";
        return 0;
    }
    return f;
}


int main() {
    char s = 'N';
    while (s == 'N') {
        if (!login()) {
            return 0;
        }
        cout << "Do you want to record the video?(Y/N)" << endl;

        char ans = 'Y';
        cin >> ans;
        
        if (c == 0){
            cout << "Where you want to store the video?[Enter the full path]" << endl;
            cin >> pa;
        }
        while (ans == 'Y') {
            c++;//video no.
            video_rec();
            cout << "Wanna Record another video?(Y/N)" << endl;

            cin >> ans;
        }
        cout << "Here is the list of video Recorded till Now-->" << endl;
        for (auto i : ls) {
            cout << i << endl;
        }
        //logout();
        cout << "do you want to logout(Y/N)?" << endl;
        cin >> s;
    }
}