#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QWidget>

#include <QVTKOpenGLNativeWidget.h>

#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkNew.h>
#include <vtkRenderer.h>
#include <vtkCylinderSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QMainWindow window;
    window.setWindowTitle("Qt + VTK BaseStation");

    // Central widget + layout
    QWidget *central = new QWidget(&window);
    QVBoxLayout *layout = new QVBoxLayout(central);
    layout->setContentsMargins(0, 0, 0, 0);

    // QVTK widget
    auto *vtkWidget = new QVTKOpenGLNativeWidget(central);
    layout->addWidget(vtkWidget);
    window.setCentralWidget(central);

    // VTK render window + renderer
    vtkNew<vtkGenericOpenGLRenderWindow> renderWindow;
    vtkWidget->setRenderWindow(renderWindow);

    vtkNew<vtkRenderer> renderer;
    renderWindow->AddRenderer(renderer);

    // Simple VTK pipeline: cylinder
    vtkNew<vtkCylinderSource> source;
    source->SetResolution(32);

    vtkNew<vtkPolyDataMapper> mapper;
    mapper->SetInputConnection(source->GetOutputPort());

    vtkNew<vtkActor> actor;
    actor->SetMapper(mapper);

    renderer->AddActor(actor);
    renderer->ResetCamera();

    window.resize(900, 700);
    window.show();

    return app.exec();
}