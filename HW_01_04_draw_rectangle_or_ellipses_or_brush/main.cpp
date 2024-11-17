#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

// 전역 변수
Mat g_imgColor, g_imgTmp;
bool g_isMousePressed = false;
int g_mouseStartX = -1, g_mouseStartY = -1;
Scalar g_color; // 드래그 동안 사용할 색상을 저장할 전역 변수
int g_drawingMode = 0; // 0: 사각형, 1: 타원, 2: 브러쉬

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
        g_isMousePressed = true;
        g_mouseStartX = x;
        g_mouseStartY = y;
        if (g_drawingMode != 2) { // 브러쉬 모드가 아닐 때만 g_imgColor 복사
            g_imgColor.copyTo(g_imgTmp); // 임시 이미지에 현재 상태 복사
        }
        g_color = randomColor(g_rng); // 드래그할 때 사용할 무작위 색상 선택
    }
    else if (event == EVENT_MOUSEMOVE && g_isMousePressed)
    {
        if (g_drawingMode == 0 || g_drawingMode == 1) {
            g_imgTmp.copyTo(g_imgColor); // 임시 이미지로 복원
        }

        if (g_drawingMode == 0) {
            Point pt1(min(g_mouseStartX, x), min(g_mouseStartY, y));
            Point pt2(max(g_mouseStartX, x), max(g_mouseStartY, y));
            rectangle(g_imgColor, pt1, pt2, g_color, -1); // 사각형 그리기
        } else if (g_drawingMode == 1) {
            ellipse(g_imgColor, Point((g_mouseStartX + x) / 2, (g_mouseStartY + y) / 2),
                    Size(abs(g_mouseStartX - x) / 2, abs(g_mouseStartY - y) / 2),
                    0, 0, 360, g_color, -1); // 타원 그리기
        } else if (g_drawingMode == 2) {
            // 브러쉬 모드에서는 바로 메인 이미지에 그림
            circle(g_imgColor, Point(x, y), 5, g_color, -1); // 브러쉬로 원 그리기
        }
        imshow("Mouse Events", g_imgColor); // 변경된 이미지 보여주기
    }
    else if (event == EVENT_LBUTTONUP)
    {
        g_isMousePressed = false;
        if (g_drawingMode == 0 || g_drawingMode == 1) {
            // 사각형 또는 타원을 마지막 상태로 그림
            Point pt1(min(g_mouseStartX, x), min(g_mouseStartY, y));
            Point pt2(max(g_mouseStartX, x), max(g_mouseStartY, y));

            if (g_drawingMode == 0) {
                rectangle(g_imgTmp, pt1, pt2, g_color, -1); // 사각형 그리기
            } else if (g_drawingMode == 1) {
                ellipse(g_imgTmp, Point((g_mouseStartX + x) / 2, (g_mouseStartY + y) / 2),
                        Size(abs(g_mouseStartX - x) / 2, abs(g_mouseStartY - y) / 2),
                        0, 0, 360, g_color, -1); // 타원 그리기
            }
            g_imgTmp.copyTo(g_imgColor); // 최종 도형을 포함한 임시 이미지를 메인 이미지로 복사
        }
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
        else if (key == 'm' || key == 'M') // m 키를 누르면 모드 전환
        {
            g_drawingMode = (g_drawingMode + 1) % 3; // 모드를 사이클링
        }
    }

    destroyAllWindows();
    return 0;
}
