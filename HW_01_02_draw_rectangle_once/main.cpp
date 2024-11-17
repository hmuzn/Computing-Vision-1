#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

// 전역 변수
Mat g_imgColor, g_imgTmp;
bool g_isMousePressed = false;
int g_mouseStartX = -1, g_mouseStartY = -1;
Scalar g_color; // 드래그 동안 사용할 색상을 저장할 전역 변수

// OpenCV 난수 생성기
RNG g_rng(getTickCount());
Scalar randomColor(RNG& rng)
{
    int icolor = (unsigned)rng;
    return Scalar(icolor&255, (icolor>>8)&255, (icolor>>16)&255);
}

// 마우스 콜백 함수
void mouse_callback(int event, int x, int y, int flags, void* param)
{
    if (event == EVENT_LBUTTONDOWN)
    {
        // 마우스 버튼이 눌렸을 때
        g_isMousePressed = true;
        g_mouseStartX = x;
        g_mouseStartY = y;
        g_imgColor.copyTo(g_imgTmp); // 임시 이미지에 현재 상태 복사
        g_color = randomColor(g_rng); // 드래그할 때 사용할 무작위 색상 선택
    }
    else if (event == EVENT_MOUSEMOVE && g_isMousePressed)
    {
        // 마우스를 드래그할 때
        g_imgTmp.copyTo(g_imgColor); // 마지막으로 확인된 상태로 이미지 복원
        Point pt1(min(g_mouseStartX, x), min(g_mouseStartY, y));
        Point pt2(max(g_mouseStartX, x), max(g_mouseStartY, y));
        rectangle(g_imgColor, pt1, pt2, g_color, -1); // 사각형 채워서 미리 그리기
        imshow("Mouse Events", g_imgColor); // 변경된 이미지 보여주기
    }
    else if (event == EVENT_LBUTTONUP)
    {
        // 마우스 버튼이 놓였을 때
        g_isMousePressed = false;
        Point pt1(min(g_mouseStartX, x), min(g_mouseStartY, y));
        Point pt2(max(g_mouseStartX, x), max(g_mouseStartY, y));
        rectangle(g_imgTmp, pt1, pt2, g_color, -1); // 임시 이미지에 최종 사각형을 그림
        g_imgTmp.copyTo(g_imgColor); // 임시 이미지를 실제 이미지로 복사
        imshow("Mouse Events", g_imgColor); // 변경된 이미지 보여주기
    }
}

int main()
{
    const int rows = 480;
    const int cols = 640;
    g_imgColor = Mat::zeros(rows, cols, CV_8UC3);

    String strWindowName = "Mouse Events";
    namedWindow(strWindowName);

    setMouseCallback(strWindowName, mouse_callback);

    while (true)
    {
        imshow(strWindowName, g_imgColor); // 변경된 부분이 없어도 계속 이미지를 보여줌
        char key = waitKey(1);
        if (key == 27) // ESC 키를 누르면 종료
        {
            break;
        }
    }

    destroyAllWindows();
    return 0;
}
