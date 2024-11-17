#include <iostream>
using namespace std;

#include <opencv2/opencv.hpp>
using namespace cv;

// 전역 변수
Mat    g_imgColor; // 컬러 이미지를 저장할 Mat 객체
bool   g_isMousePressed = false; // 마우스 버튼이 눌렸는지 여부
int    g_mouseStartX = -1; // 마우스 시작 X 좌표
int    g_mouseStartY = -1; // 마우스 시작 Y 좌표

// OpenCV 난수 생성기
RNG g_rng(getTickCount());
Scalar randomColor(RNG &g_rng)
{
    int icolor = (unsigned) g_rng;
    return Scalar(icolor&255, (icolor>>8)&255, (icolor>>16)&255); // 랜덤 색상 생성
}

// 마우스 콜백 함수
void mouse_callback(int event, int x, int y, int flags, void *param)
{
    // 랜덤 색상을 위한 정적 변수
    static Scalar color;

    // 마우스 왼쪽 버튼이 눌릴 때
    if (event == EVENT_LBUTTONDOWN)
    {
        // 플래그를 true로 설정
        g_isMousePressed = true;
        // 마우스 위치 기록
        g_mouseStartX = x;
        g_mouseStartY = y;
        // 랜덤 색상 선택
        color = randomColor(g_rng);
    }
    else if(event == EVENT_MOUSEMOVE && g_isMousePressed){
        Mat imgCopy = g_imgColor.clone(); // 이미지 복사
        rectangle(g_imgColor, Point(g_mouseStartX, g_mouseStartY), Point(x,y), color, -1); // 사각형 그리기
        imshow("Mouse Events", imgCopy); // 이미지 표시
    }
    else if(event == EVENT_LBUTTONUP){
        // 마우스 버튼이 떼어졌을 때
        g_isMousePressed = false;
        rectangle(g_imgColor, Point(g_mouseStartX, g_mouseStartY), Point(x, y), color, -1); // 사각형 그리기
    }
}

int main()
{
    // 검은색 이미지 생성
    const int rows = 480;
    const int cols = 640;
    g_imgColor = Mat::zeros(rows, cols, CV_8UC3);

    // 윈도우 생성
    String strWindowName = "Mouse Events";
    namedWindow(strWindowName);

    // 마우스 콜백 함수 등록
    setMouseCallback(strWindowName, mouse_callback);

    // 무한 루프
    while (true)
    {
        // 이미지 표시
        imshow(strWindowName, g_imgColor);

        // 사용자 입력 받기
        char key = waitKey(1);

        // ESC 키를 누르면 루프 종료
        if (key == 27) break;
    }

    // 모든 윈도우 제거
    destroyAllWindows();

    return 0;
}
