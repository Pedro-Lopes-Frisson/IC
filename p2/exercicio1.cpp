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

    Mat img_out = Mat::zeros(Size(img_in.cols,img_in.rows),img_in.type());

    Size s = img_in.size();

    // Percorre todos os pixeis da imagem
    for(int i = 0; i < s.height; i++){
        for(int j = 0; j < s.width; j++){
            img_out.at<Vec3b>(i,j) = img_in.at<Vec3b>(i,j);
        }
    }

    imwrite(output_file, img_out);

    namedWindow("Imagem Original", 1);
    imshow("Imagem Original", img_in);

    namedWindow("Imagem Copiada", 1);
    imshow("Imagem Copiada", img_out);

    waitKey(0);

    return 0;
}