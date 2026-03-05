#include <QApplication>
#include <QMainWindow>
#include <QMessageBox>
#include <QDir>
#include <QFileInfo>
#include <QSurfaceFormat>

#include <QVTKOpenGLNativeWidget.h>

#include <vtkActor.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkNamedColors.h>
#include <vtkNew.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>      // ★关键：解决 vtkProperty incomplete type
#include <vtkRenderer.h>
#include <vtkSTLReader.h>

static QString findStlPath(const QString& fileName)
{
    // 1) exe 同目录（最符合“运行程序”的方式）
    const QString exeDir = QCoreApplication::applicationDirPath();
    const QString p1 = QDir(exeDir).filePath(fileName);
    if (QFileInfo::exists(p1)) return p1;

    // 2) 当前工作目录（有时 VS 工作目录就是 build/Release）
    const QString p2 = QDir::current().filePath(fileName);
    if (QFileInfo::exists(p2)) return p2;

    // 3) 项目目录（你的 stl 在 QtBaseStation 目录：build/Release/../../greatWhite.stl）
    const QString p3 = QDir(exeDir).filePath(QString("../..%1%2").arg(QDir::separator()).arg(fileName));
    if (QFileInfo::exists(p3)) return QFileInfo(p3).absoluteFilePath();

    return {};
}

int main(int argc, char* argv[])
{
    // Qt + VTK：OpenGL 格式设置（QVTKOpenGLNativeWidget 推荐）
    QSurfaceFormat::setDefaultFormat(QVTKOpenGLNativeWidget::defaultFormat());

    QApplication app(argc, argv);

    // ====== 主窗口 + VTK Widget ======
    QMainWindow window;
    window.setWindowTitle("Qt + VTK: STL Viewer");

    auto* vtkWidget = new QVTKOpenGLNativeWidget(&window);
    window.setCentralWidget(vtkWidget);

    // ====== VTK 渲染管线 ======
    vtkNew<vtkGenericOpenGLRenderWindow> renderWindow;
    vtkWidget->setRenderWindow(renderWindow);

    vtkNew<vtkRenderer> renderer;
    renderWindow->AddRenderer(renderer);

    vtkNew<vtkNamedColors> colors;
    renderer->SetBackground(colors->GetColor3d("Black").GetData());

    // ====== 找 STL ======
    const QString stlName = "greatWhite.stl";
    const QString stlPath = findStlPath(stlName);

    if (stlPath.isEmpty())
    {
        QMessageBox::critical(nullptr, "STL not found",
            QString("Cannot find %1\n\nTried:\n- exe folder\n- current working folder\n- project folder (../..)\n\n"
                "Put %1 into QtBaseStation folder (same place as main.cpp) or into build/Release folder.")
            .arg(stlName));
        return 1;
    }

    // ====== 读取 STL ======
    vtkNew<vtkSTLReader> reader;
    reader->SetFileName(stlPath.toStdString().c_str());
    reader->Update();

    vtkNew<vtkPolyDataMapper> mapper;
    mapper->SetInputConnection(reader->GetOutputPort());

    vtkNew<vtkActor> actor;
    actor->SetMapper(mapper);

    // 简单材质
    actor->GetProperty()->SetColor(colors->GetColor3d("White").GetData());
    actor->GetProperty()->SetSpecular(0.2);
    actor->GetProperty()->SetSpecularPower(20.0);

    renderer->AddActor(actor);
    renderer->ResetCamera();

    window.resize(900, 700);
    window.show();

    return app.exec();
}