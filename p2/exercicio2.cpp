#include <iostream>
#include <opencv2/opencv.hpp>
#include <string.h>
#include <string>

using namespace std;
using namespace cv;

// Função que verifica que o número passado para o angulo é inteiro
bool isNumber(char number[])
{
    int i = 0;

    //checking for negative numbers
    if (number[0] == '-')
        i = 1;
    for (; number[i] != 0; i++)
    {
        //if (number[i] > '9' || number[i] < '0')
        if (!isdigit(number[i]))
            return false;
    }
    return true;
}


int main(int argc, char *argv[]){

    // Testar o número de inputs
    if(argc != 4){
        cout << "\033[1;31mError: Usage sintax is <in file name> <Rotation Angle> <Value for Intensity>\033[0m" << endl;
        return -1;
    }

    // Guardar a primeira imagem
    char* input_file = argv[1];

    // Verificar que o 3 argumento é interio
    if(isNumber(argv[2]) == false){
        cout << "\033[1;31m<Rotation Angle> needs to be an integer number\033[0m" << endl;
        return -1;
    }

    // Verifica se o 4 argumento é interio
    if(isNumber(argv[3]) == false){
        cout << "\033[1;31m<Value for Intensity> needs to be an integer number\033[0m" << endl;
        return -1;
    }

    // Valor para a intesidade dos píxeis
    int inten = atoi(argv[3]);

    // Verificar que o 4 argumento esta entre -255 e 255
    if(-255 > inten && inten > 255){
        cout << "\033[1;31m<Value for Intensity> needs to be between -255 and 255\033[0m" << endl;
        return -1;
    }

    // Passamos o valor do anngulo para inteiro
    int angleNum = atoi(argv[2]);

    // Guardamos os pixeis numa matrix
    Mat img_in = imread(input_file);
    // Dimensões da imagem passada como input
    Size s = img_in.size();

    // Criamos uma matrix de zeros do mmm tamnho da imagem que foi passada como input
    Mat neg_img = Mat::zeros(Size(img_in.cols,img_in.rows),img_in.type());

    // Verificamos se a imagem esta correta
    if(img_in.empty()){
        cout << "\033[1;31mError: Image provided does not exist!\033[0m" << endl;
        return -1;
    }

    // Percorre todos os pixeis da imagem
    for(int i = 0; i < s.height; i++){
        for(int j = 0; j < s.width; j++){
            // Subtrair 255 a cada píxel para fazer a imagem negativa
            subtract(255,img_in.at<Vec3b>(i, j),neg_img.at<Vec3b>(i,j));
        }
    }

    // Mirror Vertically Image
    Mat mirror_Ver_img = Mat::zeros(Size(img_in.cols,img_in.rows),img_in.type());
    cv::flip(img_in, mirror_Ver_img, 1);

    // Mirror Horizontal Image
    Mat mirror_Hor_img = Mat::zeros(Size(img_in.cols,img_in.rows),img_in.type());
    cv::flip(img_in, mirror_Hor_img, 0);

    // Aplicar a rotação de uma imagem por múltimplos de 90
    Mat rot_img = Mat::zeros(Size(img_in.cols,img_in.rows),img_in.type());
    // String para o titulo da roração
    string rot_title;

    if(angleNum >= 0){
        rot_title = strcat(argv[2], " * 90 Clockwise Rotation");
        if(angleNum == 0){
            rot_img = img_in.clone();
        }
        else if (angleNum == 1){
            cv::rotate(img_in, rot_img, cv::ROTATE_90_CLOCKWISE);
        }
        else{
            cv::rotate(img_in, rot_img, cv::ROTATE_90_CLOCKWISE);
            for(int k = 0; k < (angleNum - 1); k++){
                cv::rotate(rot_img, rot_img, cv::ROTATE_90_CLOCKWISE);
            }
        }
    }
    else{
        rot_title = strcat(argv[2], " * 90 Anti-Clockwise Rotation");
        if (abs(angleNum) == 1){
            cv::rotate(img_in, rot_img, ROTATE_90_COUNTERCLOCKWISE);
        }
        else{
            cv::rotate(img_in, rot_img, ROTATE_90_COUNTERCLOCKWISE);
            for(int k = 0; k < (abs(angleNum) - 1); k++){
                cv::rotate(rot_img, rot_img, ROTATE_90_COUNTERCLOCKWISE);
            }
        }
    }

    // Criar matrix para a intensidade
    Mat inten_img = Mat::zeros(Size(img_in.cols,img_in.rows),img_in.type());
    // String para o titulo da intensidade
    string inten_title;
    if( inten >= 0){
        inten_title = strcat(argv[3], " Intensity Incrised");
        img_in.convertTo(inten_img, -1, 1, inten); 
    }
    else{
        inten_title = strcat(argv[3], " Intensity Decrised");
        img_in.convertTo(inten_img, -1, 1, inten); 
    }

    // Mostrar Imagens
    // Original
    namedWindow("Imagem Original", 1);
    imshow("Imagem Original", img_in);
    // Negativa
    namedWindow("Imagem Negativa", 1);
    imshow("Imagem Negativa", neg_img);
    // Espelhada em relação à vertical
    namedWindow("Imagem Mirrored Vertically", 1);
    imshow("Imagem Mirrored Vertically", mirror_Ver_img);
    // Espelahda em relção à horizontal
    namedWindow("Imagem Mirrored Horizontally", 1);
    imshow("Imagem Mirrored Horizontally", mirror_Hor_img);
    // Rotação
    namedWindow(rot_title, 1);
    imshow(rot_title, rot_img);
    // Intensidade
    namedWindow(inten_title, 1);
    imshow(inten_title, inten_img);

    waitKey(0);

    return 0;
}