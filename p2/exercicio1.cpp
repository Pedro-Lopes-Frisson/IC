#include <iostream>
#include <opencv2/opencv.hpp>
#include <string.h>

using namespace std;
using namespace cv;

int main(int argc, char *argv[]){
    if(argc != 3){
        cout << "\033[1;31mError: Usage sintax is <in file name> <out file name>\033[0m" << endl;
        return -1;
    }

    char* input_file = argv[1];
    char* output_file = argv[2];

    Mat img_in = imread(input_file);

    if(img_in.empty()){
        cout << "\033[1;31mError: Image provided does not exist!\033[0m" << endl;
        return -1;
    }

    Mat img_out = Mat::zeros(Size(image.cols,image.rows),img_in.type());

    int startrow, startcolumn;
    
    startrow = 0;
    startcolumn = 0;

    int rows, columns, i, j, r = 0, c;

    Size s = img_in.size();
    rows = s.height;
    columns = s.width;

    for(i = startrow; i < startrow + rows; i++){
        c = 0;
        for(j = startcolumn; j < startcolumn + columns; j++){
            img_out.at<Vec3b>(i,j) = img_in.at<Vec3b>(r,c);
            c++;
        }
        r++;
    }
    imwrite(output_file, img_out);
    imshow("Output", img_out);
    waitKey(0);
    return 0;
}