#include "mainframe.h"
#include "ui_mainframe.h"
#include "imageform.h"
#include "circleHough.h"
#include "edge.h"
#include "corner.h"

#include <QFileDialog>
#include <QPainter>
#include <sstream>
#include <fstream>
#include <QMessageBox>


using namespace std;

MainFrame::MainFrame(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainFrame)
{
    ui->setupUi(this);

    _q_pFormFocused     = 0;

    //객체 맴버의 초기화


    //리스트 출력창을 안보이게    
    ui->listWidget->setVisible(false);
    this->adjustSize();

    //UI 활성화 갱신
    UpdateUI();
}

MainFrame::~MainFrame()
{ 
    delete ui;         

    for(auto& item : _lImageForm)
        delete item;

}

void MainFrame::CloseImageForm(ImageForm *pForm)
{
    //ImageForm 포인터 삭제
    unsigned int idx = std::find(_lImageForm.begin(), _lImageForm.end(), pForm) - _lImageForm.begin();
    if(idx != _lImageForm.size())
    {
        delete _lImageForm[idx];
        _lImageForm.erase(_lImageForm.begin() + idx);
    }

    //활성화 ImageForm 초기화
    _q_pFormFocused     = 0;

    //관련 객체 삭제

    //UI 활성화 갱신
    UpdateUI();
}

void MainFrame::UpdateUI()
{    
    if(ui->tabWidget->currentIndex() == 0)
    {
    }
    else if(ui->tabWidget->currentIndex() == 1)
    {  
        ui->pushSepiaTone->setEnabled(_q_pFormFocused &&
                                      _q_pFormFocused->ID() == "OPEN");
        ui->pushContrast->setEnabled(_q_pFormFocused &&
                                     _q_pFormFocused->ID() == "OPEN");
    }
    else if(ui->tabWidget->currentIndex() == 2)
    {        

    }
    else if(ui->tabWidget->currentIndex() == 3)
    {
    }
}

void MainFrame::OnMousePos(const int &nX, const int &nY, ImageForm* q_pForm)
{
}

void MainFrame::focusInEvent(QFocusEvent * event)
{
    Q_UNUSED(event) ;

    UpdateUI();
}

void MainFrame::closeEvent(QCloseEvent* event)
{
    //생성된 ImageForm을 닫는다.
    for(int i=0; i< _lImageForm.size(); i++)
        delete _lImageForm[i];

    //리스트에서 삭제한다.
    _lImageForm.clear();
}


void MainFrame::on_buttonOpen_clicked()
{
    //이미지 파일 선택
    QFileDialog::Options    q_Options   =  QFileDialog::DontResolveSymlinks  | QFileDialog::DontUseNativeDialog; // | QFileDialog::ShowDirsOnly
    QString                 q_stFile    =  QFileDialog::getOpenFileName(this, tr("Select a Image File"),  "./data", "Image Files(*.bmp *.ppm *.pgm *.png)",0, q_Options);

    if(q_stFile.length() == 0)
        return;

    //이미지 출력을 위한 ImageForm 생성    
    ImageForm*              q_pForm   = new ImageForm(q_stFile, "OPEN", this);

    _lImageForm.push_back(q_pForm);
    q_pForm->show();

    //UI 활성화 갱신
    UpdateUI();
}

void MainFrame::on_buttonDeleteContents_clicked()
{
    //생성된 ImageForm을 닫는다.
    for(int i=_lImageForm.size()-1; i>=0; i--)
        delete _lImageForm[i];

    //리스트에서 삭제한다.
    _lImageForm.clear();

    //객체 삭제


    ui->listWidget->clear();
}

void MainFrame::on_tabWidget_currentChanged(int index)
{
    static int nOld = -1;

    if(nOld == 0)
    {

    }
    else if(nOld == 1)
    {

    }
    nOld = index;

    //UI 활성화 갱신
    UpdateUI();
}

void MainFrame::on_buttonShowList_clicked()
{
    static int nWidthOld = ui->tabWidget->width();

    if(ui->listWidget->isVisible())
    {
        nWidthOld = ui->listWidget->width();
        ui->listWidget->hide();
        this->adjustSize();
    }
    else
    {        
        ui->listWidget->show();
        QRect q_rcWin = this->geometry();

        this->setGeometry(q_rcWin.left(), q_rcWin.top(), q_rcWin.width()+nWidthOld, q_rcWin.height());
    }
}




void MainFrame::on_pushContrast_clicked()
{
    //포커스 된 ImageForm으로부터 영상을 가져옴
    KImageGray igImg;
    if(_q_pFormFocused == 0 || _q_pFormFocused->ImageGray().Address() == 0 || _q_pFormFocused->ID() != "OPEN")
        return;

    //포커스된 ImageForm의 영상을 Contrast Transform
    igImg = _q_pFormFocused->ImageGray().ContrastTransform(ui->spinContrastA->value(),
                                                                      ui->spinContrastB->value(),0,255);

    //출력창 결정
    ImageForm* q_pForm = 0;
    for(auto item : _lImageForm)
        if(item->ID() == "Contrast Transform" && item->size() == _q_pFormFocused->size())
        {
            q_pForm = item;
            break;
        }

    if(q_pForm)
        q_pForm->Update(igImg);
    else
    {
        q_pForm = new ImageForm(igImg, "Contrast Transform", this);
        q_pForm->show();
        _lImageForm.push_back(q_pForm);
    }

    UpdateUI();
}


void MainFrame::on_pushSepiaTone_clicked()
{
    KImageColor icMain;
    if(_q_pFormFocused != 0 && _q_pFormFocused->ImageColor().Address() && _q_pFormFocused->ID() == "OPEN")
        icMain = _q_pFormFocused->ImageColor();
    else
        return;

    KArray<KHSV> arHSV;
    icMain.RGBtoHSV(arHSV);

    double dHue = ui->spinHue->value();
    double dSat = ui->spinSat->value();

    for(unsigned int i = 0; i<arHSV.Row(); i++)
        for(unsigned int j = 0; j<arHSV.Col(); j++)
        {
            arHSV[i][j].h = dHue;
            arHSV[i][j].s = dSat;
        }

    icMain.FromHSV(arHSV);

    ImageForm* q_pForm = 0;
    for(auto item : _lImageForm)
        if(item->ID() == "Sepia Tone" && item->size() == _q_pFormFocused->size())
        {
            q_pForm = item;
            break;
        }

    if(q_pForm)
        q_pForm->Update(icMain);
    else
    {
        q_pForm = new ImageForm(icMain, "Sepia Tone", this);
        q_pForm->show();
        _lImageForm.push_back(q_pForm);
    }

    UpdateUI();

}

void MainFrame::on_pushDilation_clicked()
{
    //이미지 가져오기
    int masksize = 3;
    if(ui->S5by5_2->isChecked())
        masksize = 5;
    KImageGray igBin = _q_pFormFocused->ImageGray().BinaryDilate(_FOREGROUND, masksize);
    //출력창결정
    ImageForm* q_pForm = 0;
    for(auto item : _lImageForm)
    {
        if(item->ID()=="Dilation Image" && item->size() == _q_pFormFocused->size())
        {
            q_pForm = item;
            break;
        }
    }
    //기존창에 표시
    if(q_pForm)
        q_pForm->Update(igBin);
    else
    {
        q_pForm = new ImageForm(igBin, "Dilation Image", this);
        q_pForm->show();
        _lImageForm.push_back(q_pForm);

    }

    UpdateUI();


}


void MainFrame::on_pushErosion_clicked()
{
    int masksize = 3;
    if(ui->S5by5_2->isChecked())
        masksize = 5;
    //영상 가져오기
    KImageGray igBin = _q_pFormFocused->ImageGray().BinaryErode(_FOREGROUND, masksize);
    //출력창결정
    ImageForm* q_pForm = 0;
    for(auto item : _lImageForm)
    {
        if(item->ID()=="Erode Image" && item->size() == _q_pFormFocused->size())
        {
            q_pForm = item;
            break;
        }
    }

    //기존창에 표시
    if(q_pForm)
        q_pForm->Update(igBin);
    else
    {
        q_pForm = new ImageForm(igBin, "Erode Image", this);
        q_pForm->show();
        _lImageForm.push_back(q_pForm);
    }

    UpdateUI();
}


void MainFrame::on_pushOstu_clicked()
{
    //Ostu Thresholding
    KImageGray igMain = _q_pFormFocused->ImageGray();
    KImageGray igBin;
    KBINARIZATION_OUTPUT* opBinOutput = KHisto().Ostu(&igMain, &igBin);

    ImageForm* q_pForm = 0;
    for(auto item : _lImageForm)
    {
        if(item->ID()=="Binary Image" && item->size() == _q_pFormFocused->size())
        {
            q_pForm = item;
            break;
        }
    }

    //기존창에 표시
    if(q_pForm)
        q_pForm->Update(igBin);
    else
    {
        q_pForm = new ImageForm(igBin, "Binary Image", this);
        q_pForm->show();
        _lImageForm.push_back(q_pForm);
    }

    ui->spinOstu->setValue(opBinOutput->nThresh);
    ui->listWidget->addItem(QString(">> Ostu ThresHolding : %1").arg(opBinOutput->nThresh));
    ui->listWidget->show();

    UpdateUI();
}


void MainFrame::on_pushThresholding_clicked()
{
    KImageGray igMain = _q_pFormFocused->ImageGray();
    igMain.Thresholded(ui->spinOstu->value());

    ImageForm* q_pForm = 0;
    for(auto item : _lImageForm)
    {
        if(item->ID()=="Binary Image" && item->size() == _q_pFormFocused->size())
        {
            q_pForm = item;
            break;
        }
    }

    //기존창에 표시
    if(q_pForm)
        q_pForm->Update(igMain);
    else
    {
        q_pForm = new ImageForm(igMain, "Binary Image", this);
        q_pForm->show();
        _lImageForm.push_back(q_pForm);
    }

    ui->listWidget->addItem(QString(">> Manual Thresholding : %1").arg(ui->spinOstu->value()));
    ui->listWidget->show();

    UpdateUI();
}





void MainFrame::on_pushOpening_clicked()
{

    KImageGray igDil;
    KImageGray igEro;
    if(ui->S3by3->isChecked()){
        igEro = _q_pFormFocused->ImageGray().BinaryErode(_WHITE,3);
        igDil = igEro.BinaryDilate(_WHITE,3);
    }
    else if(ui->S5by5->isChecked()){
        igEro = _q_pFormFocused->ImageGray().BinaryErode(_WHITE,5);
        igDil = igEro.BinaryDilate(_WHITE,5);
    }
    ImageForm* q_pForm = nullptr;
    for(auto item : _lImageForm)
    {
        if(item->ID()=="Opening Image" && item->size() == _q_pFormFocused->size())
        {
            q_pForm = item;
            break;
        }
    }

    //기존창에 표시
    if(q_pForm)
        q_pForm->Update(igDil);
    else
    {
        q_pForm = new ImageForm(igDil, "Opening Image", this);
        q_pForm->show();
        _lImageForm.push_back(q_pForm);
    }



    UpdateUI();
}


void MainFrame::on_pushClosing_clicked()
{


    KImageGray igDil;
    KImageGray igEro;
    if(ui->S3by3->isChecked()){
        igDil= _q_pFormFocused->ImageGray().BinaryDilate(_WHITE,3);
        igEro = igDil.BinaryErode(_WHITE,3);
    }
    else if(ui->S5by5->isChecked()){
        igDil = _q_pFormFocused->ImageGray().BinaryDilate(_WHITE,5);
        igEro = igDil.BinaryErode(_WHITE,5);
    }

    ImageForm* q_pForm = nullptr;
    for(auto item : _lImageForm)
    {
        if(item->ID()=="Closing Image" && item->size() == _q_pFormFocused->size())
        {
            q_pForm = item;
            break;
        }
    }

    //기존창에 표시
    if(q_pForm)
        q_pForm->Update(igEro);
    else
    {
        q_pForm = new ImageForm(igEro, "Closing Image", this);
        q_pForm->show();
        _lImageForm.push_back(q_pForm);
    }
    UpdateUI();

}




void MainFrame::on_pushHEqu_clicked()
{
    KImageGray igMain = _q_pFormFocused->ImageGray();




    igMain.HistoEqualize();

    ImageForm* q_pForm = 0;
    for(auto item : _lImageForm)
    {
        if(item->ID()=="Equalization Image" && item->size() == _q_pFormFocused->size())
        {
            q_pForm = item;
            break;
        }
    }

    //기존창에 표시
    if(q_pForm)
        q_pForm->Update(igMain);
    else
    {
        q_pForm = new ImageForm(igMain, "Equalization Image", this);
        q_pForm->show();
        _lImageForm.push_back(q_pForm);
    }
    UpdateUI();
}


void MainFrame::on_pushRGB2HSI_clicked()
{
    KImageColor icMain;
    if(_q_pFormFocused != 0 && _q_pFormFocused->ImageColor().Address() && _q_pFormFocused->ID() == "OPEN")
        icMain = _q_pFormFocused->ImageColor();
    else
        return;

    KArray<KHSI> arHSI;

    icMain.RGBtoHSI(arHSI);





    KImageGray igHue(icMain.Row(), icMain.Col()),
               igSat(icMain.Row(), icMain.Col()),
               igint(icMain.Row(), icMain.Col());


    for (unsigned int i = 0; i < icMain.Row(); ++i) {
           for (unsigned int j = 0; j < icMain.Col(); ++j) {
               // Hue 값 (0~255로 변환)
               igHue[i][j] = (unsigned char)(arHSI[i][j].h*255.0);

               // Saturation 값 (0~255로 변환)
               igSat[i][j] = (unsigned char)(arHSI[i][j].s*255.0);

               // Intensity 값 (0~255로 변환)
               igint[i][j] = (unsigned char)(arHSI[i][j].i*255.0);
           }
       }




    ImageForm* q_pHue = new ImageForm(igHue,"Hue Image", this);
    q_pHue->show();
    _lImageForm.push_back(q_pHue);

    ImageForm* q_pSat = new ImageForm(igSat, "Sat Image", this);
    q_pSat->show();
    _lImageForm.push_back(q_pSat);

    ImageForm* q_pInt = new ImageForm(igint, "Int Image", this);
    q_pInt->show();
    _lImageForm.push_back(q_pInt);




}


void MainFrame::on_pushRGB2NRG_clicked()
{
        KImageColor   icMain;

        // 포커스 된 ImageForm으로부터 영상을 가져옴
        if (_q_pFormFocused != 0 && _q_pFormFocused->ImageColor().Address() && _q_pFormFocused->ID() == "OPEN")
            icMain = _q_pFormFocused->ImageColor();
        else
            return;

        // Normalized Red, Green 값을 저장할 그레이 이미지 생성
        KImageGray img_NormalizedRed(icMain.Row(), icMain.Col());
        KImageGray img_NormalizedGreen(icMain.Row(), icMain.Col());

        double dR, dG, dB, dSum;

        // 각 픽셀의 R, G 값을 정규화하고 그레이 이미지로 변환
        for (unsigned int i = 0; i < icMain.Row(); ++i) {
            for (unsigned int j = 0; j < icMain.Col(); ++j) {
                // RGB 값을 0~1로 정규화
                dR = (double)icMain[i][j].r / 255.0;
                dG = (double)icMain[i][j].g / 255.0;
                dB = (double)icMain[i][j].b / 255.0;

                // RGB 값의 합 계산
                dSum = dR + dG + dB;

                // Normalized Red 계산 (0~255 스케일)
                if (dSum > 0)
                    img_NormalizedRed[i][j] = (unsigned char)((dR / dSum) * 255.0);
                else
                    img_NormalizedRed[i][j] = 0;  // R, G, B가 모두 0인 경우

                // Normalized Green 계산 (0~255 스케일)
                if (dSum > 0)
                    img_NormalizedGreen[i][j] = (unsigned char)((dG / dSum) * 255.0);
                else
                    img_NormalizedGreen[i][j] = 0;  // R, G, B가 모두 0인 경우
            }
        }

        // Normalized Red 이미지 출력
        ImageForm* q_pRed = new ImageForm(img_NormalizedRed, "Normalized Red Channel", this);
        q_pRed->show();
        _lImageForm.push_back(q_pRed);

        // Normalized Green 이미지 출력
        ImageForm* q_pGreen = new ImageForm(img_NormalizedGreen, "Normalized Green Channel", this);
        q_pGreen->show();
        _lImageForm.push_back(q_pGreen);

        // UI 활성화 갱신
        UpdateUI();
}


void MainFrame::on_pushCanny_clicked()
{
    KImageGray igMain = _q_pFormFocused->ImageGray();

    KImageGray igEdge;
    double dSigma = ui->editSigmaCanny->text().toDouble();
    double dLow = ui->editLow->text().toDouble();
    double dHigh = ui->editHigh->text().toDouble();
    KEdge oEdge(dSigma);

    oEdge.Canny(dLow, dHigh, igMain, igEdge);

    ImageForm* q_pForm = 0;
    for(auto item : _lImageForm)
    {
        if(item->ID()=="Edge Image" && item->size() == _q_pFormFocused->size())
        {
            q_pForm = item;
            break;
        }
    }

    //기존창에 표시
    if(q_pForm)
        q_pForm->Update(igEdge);
    else
    {
        q_pForm = new ImageForm(igMain, "Edge Image", this);
        q_pForm->show();
        _lImageForm.push_back(q_pForm);
    }
    UpdateUI();
}



void MainFrame::on_pushLabeling_clicked()
{

}


void MainFrame::on_pushCircleLoc_clicked()
{
    KImageGray igMain = _q_pFormFocused->ImageGray();
    KCIRCLEHOUGH_INFO oInfo;

    oInfo.nLeft = ui->editMinR->text().toInt();
    oInfo.nTop = ui->editMinR->text().toInt();
    oInfo.nRight = igMain.Col() - ui->editMinR->text().toInt();
    oInfo.nBottom = igMain.Row() - ui->editMinR->text().toInt();
    oInfo.nMinR = ui->editMinR->text().toInt();
    oInfo.nMaxR = ui->editMaxR->text().toInt();
    oInfo.nNumGridCx = (int)((oInfo.nRight - oInfo.nLeft + 1)/ui->editResolution->text().toDouble());
    oInfo.nNumGridCy = (int)((oInfo.nBottom - oInfo.nTop + 1)/ui->editResolution->text().toDouble());
    oInfo.nNumGridR = (int)((oInfo.nMaxR - oInfo.nMinR + 1)/ui->editResolution->text().toDouble());
    oInfo.nDirEdge = _OUTWARD_EDGE;
    oInfo.nThreshVote = ui->editThreshVote->text().toInt();

    //Edge Detection
    KEdge oEdge(ui->editSigmaCanny->text().toDouble());
    KImageGray igEdge;
    oEdge.Canny(ui->editLow->text().toInt(), ui->editHigh->text().toInt(), igMain, igEdge);

    KPGM(igEdge).Save("C:/Users/mario/Documents/Homework_CV/00 - PracticeCV-students/output/edge4Hough.pgm");

    KCircleHough oCircleHough;

    oCircleHough.Init((void*)&oInfo);
    vector<KCircle>* opCircles = (vector<KCircle>*)oCircleHough.Run((void*)&oEdge);
    if(opCircles == nullptr || opCircles->empty()){
        if(ui->listWidget->isVisible()==false)
            on_buttonShowList_clicked();
        ui->listWidget->insertItem(0, QString("There is no circle detected !"));

        return;
    }

    ImageForm* q_pForm = 0;
    for(auto item : _lImageForm)
        if(item->ID() == "Circle Localization" && item->size() == _q_pFormFocused->size()){
            q_pForm = item;
            q_pForm->Update(igMain.GrayToRGB());
            break;
        }
    if(q_pForm == 0){
        q_pForm = new ImageForm(igMain.GrayToRGB(), "Circle Localization", this);
        _lImageForm.push_back(q_pForm);
    }

    for(const auto& circle : *opCircles){
    q_pForm->DrawEllipse(QPoint((int)(circle._dCx), (int)(circle._dCy)),
                         (int)(circle._dRadius),
                         (int)(circle._dRadius),
                         QColor(255, 0, 0), 2);

    if(ui->listWidget->isVisible() == false)
        on_buttonShowList_clicked();
    ui->listWidget->insertItem(0, QString(">> detected Circle : Radius = %1").arg(circle._dRadius));
    }

    q_pForm->update();
    q_pForm->show();




    UpdateUI();




}

void MainFrame::on_pushGHT_clicked()
{
    KImageGray igMain = _q_pFormFocused->ImageGray();
    KCIRCLEHOUGH_INFO oInfo;

    //Edge Detection
    KEdge oEdge(ui->editSigmaCanny->text().toDouble());
    KImageGray igEdge;
    oEdge.Canny(ui->editLow->text().toInt(), ui->editHigh->text().toInt(), igMain, igEdge);

    oInfo.nLeft = 0;
    oInfo.nTop = 0;
    oInfo.nRight = igMain.Col();
    oInfo.nBottom = igMain.Row();
    oInfo.nMinR = 5;
    oInfo.nMaxR = 50;
    oInfo.nNumGridCx = 200;
    oInfo.nNumGridCy = 200;
    oInfo.nNumGridR = 10;
    oInfo.nDirEdge = _OUTWARD_EDGE;
    oInfo.nThreshVote = ui->editThreshVote->text().toInt();

    KCircleHough ght;
    ght.Init(&oInfo);

    ifstream maskFile("C:/mask.txt");
    vector<KPOINT> maskPoints;
    int x, y, numP;
    maskFile >> numP;
    while(maskFile >> x >> y){
        maskPoints.push_back({x, y});
    }
    maskFile.close();
    if (maskPoints.empty()) {
        QMessageBox::critical(this, "Error", "No valid points loaded from mask file!");
        return;
    }

    int maskCenterX = 0, maskCenterY = 0;
        for (const auto& point : maskPoints) {
            maskCenterX += point.nX;
            maskCenterY += point.nY;
        }
        maskCenterX /= maskPoints.size();
        maskCenterY /= maskPoints.size();
        qDebug() << "Mask Original Center (" << maskCenterX << "," << maskCenterY << ")";
        for (auto& point : maskPoints) {
            point.nX -= maskCenterX;
            point.nY -= maskCenterY;
        }

    ght.maskVectors = maskPoints;

    KPOINT* dCenter = (KPOINT*)ght.Run1(&oEdge);
    if (!dCenter) {
        QMessageBox::warning(this, "Error", "No object detected during GHT execution!");
        return;
    }

    if (dCenter->nX < 0 || dCenter->nX >= igMain.Col() ||
           dCenter->nY < 0 || dCenter->nY >= igMain.Row()) {
           QMessageBox::warning(this, "Error", "Detected center is out of image bounds!");
           delete dCenter;
           return;
       }

    ImageForm* q_pForm = 0;
    for(auto item : _lImageForm)
        if(item->ID() == "Generalized Hough Transform" && item->size() == _q_pFormFocused->size()){
            q_pForm = item;
            q_pForm->Update(igMain.GrayToRGB());
            break;
        }
    if(q_pForm == 0){
        q_pForm = new ImageForm(igMain.GrayToRGB(), "Generalized Hough Transform", this);
        _lImageForm.push_back(q_pForm);
    }

    vector<double> edgeAngles;
    for(auto& edge : oEdge){
        if(abs(edge.u - dCenter->nX) < 10 && abs(edge.v - dCenter->nY))
            edgeAngles.push_back(edge.wDir * M_PI / 180.0);
    }
    double sinSum = 0.0, cosSum = 0.0;
    for(double angle : edgeAngles){
        sinSum += sin(angle);
        cosSum += cos(angle);
    }
    double detectedAngle = atan2(sinSum, cosSum);

    double scaleX = (igMain.Col()) / oInfo.nNumGridCx;
    double scaleY = (igMain.Row()) / oInfo.nNumGridCy;

    QPolygon polygon;
            for (const auto& point : maskPoints) {
                double rotatedX = point.nX * cos(detectedAngle) - point.nY * sin(detectedAngle);
                double rotatedY = point.nX * sin(detectedAngle) + point.nY * cos(detectedAngle);
                int transformedX = (dCenter->nX + point.nX)+rotatedX;
                int transformedY = (dCenter->nY + point.nY)+rotatedY;
                polygon << QPoint(transformedX, transformedY);
                qDebug() << "Transformed Point: " << transformedX << ", " << transformedY;
            }



    _q_pFormFocused->DrawPolygon(polygon,
                         QColor(255, 0, 0), 2);
    if (polygon.isEmpty()) {
        QMessageBox::warning(this, "Error", "Generated polygon is empty!");
        return;
    }




    if(ui->listWidget->isVisible() == false)
        on_buttonShowList_clicked();


    q_pForm->update();
    q_pForm->show();




    UpdateUI();

    delete dCenter;




}



