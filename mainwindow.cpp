#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "editprofile.h"
#include "ui_editprofile.h"
#include "newprofile.h"
#include "plotwidget.h"
#include "amdpstateeditor.h"
#ifdef NVIDIA
#include <NVCtrl/NVCtrl.h>
#endif

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    checkForRoot();
    checkForProfiles();
    tabHandler(ui->tabWidget->currentIndex());

    // Create persistent nvidia pointer
#ifdef NVIDIA
    nv = new nvidia;
    types = nv;
#endif
#ifdef AMD
    amdptr = new amd;
    types = amdptr;
#endif
    // Set pointer locations for ui elements
    types->fanSlider = ui->fanSlider;
    types->fanSpinBox = ui->fanSpinBox;
    types->fanLabel = ui->fanSpeedLabel;

    types->voltageSlider = ui->voltageSlider;
    types->voltageSpinBox = ui->voltageSpinBox;
    types->voltageLabel = ui->voltgeLabel;

    types->coreClockSlider = ui->frequencySlider;
    types->coreClockSpinBox = ui->frequencySpinBox;
    types->coreClockLabel = ui->clockFreqLabel;

    types->powerLimSlider = ui->powerLimSlider;
    types->powerLimSpinBox = ui->powerLimSpinBox;
    types->powerLimLabel = ui->powerLimLabel;

    types->memClockSlider = ui->memClkSlider;
    types->memClockSpinBox = ui->memClkSpinBox;
    types->memClockLabel = ui->memClockLabel;

    // This is for libxnvctrl
    types->setupGPU();
    // This is for NVML
    types->setupGPUSecondary(currentGPUIndex);
    types->queryGPUFeatures();
    //types->queryGPUFrequencies(currentGPUIndex);
    //types->queryGPUFreqOffset(currentGPUIndex);
    //types->queryGPUMemClkOffset(currentGPUIndex);
    //types->queryGPUVoltageOffset(currentGPUIndex);
    //types->queryGPUPowerLimit(currentGPUIndex);
    //types->queryGPUPowerLimitAvailability(currentGPUIndex);
    //types->queryGPUPowerLimitLimits(currentGPUIndex);
    //types->queryGPUCurrentMaxClocks(currentGPUIndex);
    if (types->GPUList[currentGPUIndex].gputype == types->AMDGPU) {
        types->calculateUIProperties(currentGPUIndex);
    }
    // Populate the GPU combo box
    for (int i=0; i<types->gpuCount; i++) {
        ui->GPUComboBox->addItem("GPU-" + QString::number(i) + ": " + types->GPUList[i].displayName);
    }

    //loadProfileSettings();
    //setupMonitorTab();
    setupGraphMonitorTab();


    // Enable sliders according to GPU properties
    /*
    if (types->GPUList[currentGPUIndex].overClockAvailable) {
        ui->frequencySlider->setRange(types->GPUList[currentGPUIndex].minCoreClkOffset, types->GPUList[currentGPUIndex].maxCoreClkOffset);
        ui->frequencySpinBox->setRange(types->GPUList[currentGPUIndex].minCoreClkOffset, types->GPUList[currentGPUIndex].maxCoreClkOffset);
        ui->frequencySlider->setValue(types->GPUList[currentGPUIndex].coreClkOffset);
        ui->frequencySpinBox->setValue(types->GPUList[currentGPUIndex].coreClkOffset);
    } else {
        ui->frequencySlider->setEnabled(false);
        ui->frequencySpinBox->setEnabled(false);
    }

    if (types->GPUList[currentGPUIndex].memOverClockAvailable) {
        // Divide by 2 to get the clock speed
        ui->memClkSlider->setRange(types->GPUList[currentGPUIndex].minMemClkOffset/2, types->GPUList[currentGPUIndex].maxMemClkOffset/2);
        ui->memClkSpinBox->setRange(types->GPUList[currentGPUIndex].minMemClkOffset/2, types->GPUList[currentGPUIndex].maxMemClkOffset/2);
        ui->memClkSlider->setValue(types->GPUList[currentGPUIndex].memClkOffset);
        ui->memClkSpinBox->setValue(types->GPUList[currentGPUIndex].memClkOffset);
    } else {
        ui->memClkSlider->setEnabled(false);
        ui->memClkSpinBox->setEnabled(false);
    }

    if (types->GPUList[currentGPUIndex].powerLimitAvailable) {
        ui->powerLimSlider->setRange(types->GPUList[currentGPUIndex].minPowerLim/1000, types->GPUList[currentGPUIndex].maxPowerLim/1000);
        ui->powerLimSpinBox->setRange(types->GPUList[currentGPUIndex].minPowerLim/1000, types->GPUList[currentGPUIndex].maxPowerLim/1000);
        ui->powerLimSlider->setValue(types->GPUList[currentGPUIndex].powerLim/1000);
        ui->powerLimSpinBox->setValue(types->GPUList[currentGPUIndex].powerLim/1000);
    } else {
        ui->powerLimSlider->setEnabled(false);
        ui->powerLimSpinBox->setEnabled(false);
    }

    if (types->GPUList[currentGPUIndex].overVoltAvailable) {
        ui->voltageSlider->setRange(types->GPUList[currentGPUIndex].minVoltageOffset/1000, types->GPUList[currentGPUIndex].maxVoltageOffset/1000);
        ui->voltageSpinBox->setRange(types->GPUList[currentGPUIndex].minVoltageOffset/1000, types->GPUList[currentGPUIndex].maxVoltageOffset/1000);
        ui->voltageSlider->setValue(types->GPUList[currentGPUIndex].voltageOffset/1000);
        ui->voltageSpinBox->setValue(types->GPUList[currentGPUIndex].voltageOffset/1000);
    } else {
        ui->voltageSlider->setEnabled(false);
        ui->voltageSpinBox->setEnabled(false);
    }
    qDebug() << "current fanmode" << types->GPUList[currentGPUIndex].fanControlMode;
    ui->fanSlider->setValue(types->GPUList[currentGPUIndex].fanSpeed);
    ui->fanSpinBox->setValue(types->GPUList[currentGPUIndex].fanSpeed);
    ui->fanSlider->setRange(0, 100);
    ui->fanSpinBox->setRange(0, 100);

    if (types->GPUList[currentGPUIndex].fanControlMode !=1) {
        ui->fanSlider->setDisabled(true);
        ui->fanSpinBox->setDisabled(true);
    }*/

    // Testing code

    if (types->GPUList[currentGPUIndex].overVoltAvailable) {
        ui->voltageSlider->setEnabled(true);
        ui->voltageSpinBox->setEnabled(true);
        ui->voltageSlider->setRange(types->GPUList[currentGPUIndex].voltageSliderMin, types->GPUList[currentGPUIndex].voltageSliderMax);
        ui->voltageSpinBox->setRange(types->GPUList[currentGPUIndex].voltageSliderMin, types->GPUList[currentGPUIndex].voltageSliderMax);
        ui->voltageSlider->setValue(types->GPUList[currentGPUIndex].voltageSliderCur);
    } else {
        ui->voltageSlider->setEnabled(false);
        ui->voltageSpinBox->setEnabled(false);
    }

    if (types->GPUList[currentGPUIndex].overClockAvailable) {
        ui->frequencySlider->setEnabled(true);
        ui->frequencySpinBox->setEnabled(true);
        ui->frequencySpinBox->setRange(types->GPUList[currentGPUIndex].coreClkSliderMin, types->GPUList[currentGPUIndex].coreClkSliderMax);
        ui->frequencySlider->setRange(types->GPUList[currentGPUIndex].coreClkSliderMin, types->GPUList[currentGPUIndex].coreClkSliderMin);
        ui->frequencySlider->setValue(types->GPUList[currentGPUIndex].coreClkSliderCur);

        ui->memClkSlider->setEnabled(true);
        ui->memClkSpinBox->setEnabled(true);
        ui->memClkSlider->setRange(types->GPUList[currentGPUIndex].memClkSliderMin, types->GPUList[currentGPUIndex].memClkSliderMax);
        ui->memClkSpinBox->setRange(types->GPUList[currentGPUIndex].memClkSliderMin, types->GPUList[currentGPUIndex].memClkSliderMax);
        ui->memClkSlider->setValue(types->GPUList[currentGPUIndex].memClkSliderCur);
    } else {
        ui->frequencySlider->setEnabled(false);
        ui->frequencySpinBox->setEnabled(false);
        ui->memClkSlider->setEnabled(false);
        ui->memClkSpinBox->setEnabled(false);
    }

    if (types->GPUList[currentGPUIndex].powerLimitAvailable) {
        ui->powerLimSlider->setEnabled(true);
        ui->powerLimSpinBox->setEnabled(true);
        ui->powerLimSlider->setRange(types->GPUList[currentGPUIndex].powerLimSliderMin, types->GPUList[currentGPUIndex].powerLimSliderMax);
        ui->powerLimSpinBox->setRange(types->GPUList[currentGPUIndex].powerLimSliderMin, types->GPUList[currentGPUIndex].powerLimSliderMax);
        ui->powerLimSlider->setValue(types->GPUList[currentGPUIndex].powerLimSliderCur);
    } else {
        ui->powerLimSlider->setEnabled(false);
        ui->powerLimSpinBox->setEnabled(false);
    }
    /*if (!types->GPUList[currentGPUIndex].manualFanCtrlAvailable) {
        // If manual fan control is not available for the GPU, disable the option
        QStandardItemModel *model = qobject_cast<QStandardItemModel*>(ui->fanModeComboBox->model());
        QModelIndex manualModeIndex = model->index(1, ui->fanModeComboBox->modelColumn());
        QStandardItem *manualMode = model->itemFromIndex(manualModeIndex);
        manualMode->setEnabled(false);
        manualMode->setToolTip("Manual fan control is not available for current GPU");
    }
*/
    connect(fanUpdateTimer, SIGNAL(timeout()), this, SLOT(fanSpeedUpdater()));
    fanUpdateTimer->start(2000);

    connect(ui->frequencySpinBox, SIGNAL(valueChanged(int)), SLOT(resetTimer()));
    connect(ui->powerLimSpinBox, SIGNAL(valueChanged(int)), SLOT(resetTimer()));
    connect(ui->memClkSpinBox, SIGNAL(valueChanged(int)), SLOT(resetTimer()));
    connect(ui->voltageSpinBox, SIGNAL(valueChanged(int)), SLOT(resetTimer()));

    connect(ui->tabWidget, SIGNAL(currentChanged(int)), SLOT(tabHandler(int)));
    connect(monitorUpdater, SIGNAL(timeout()), SLOT(updateMonitor()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionEdit_current_profile_triggered(bool)
{
    editProfile *editprof = new editProfile(this);
    editprof->show();
}

void MainWindow::checkForRoot()
{
    QProcess process;
    process.start("/bin/sh -c \"echo $EUID\"");
    process.waitForFinished();
    QString EUID = process.readLine();
    if (EUID.toInt() == 0) {
        isRoot = true;
        qDebug() << "Running as root";
    } else {
        qDebug() << "Running as normal user";
    }
}
void MainWindow::tabHandler(int index)
{
    // Disconnect the monitor updater when the tab is not visible
    // Maybe do this with ifs if the tabs can be moved
    switch (index) {
        case 2:
            monitorUpdater->start(1000);
            break;
        case 1:
            monitorUpdater->start(1000);
            break;
        default:
            monitorUpdater->stop();
            break;
    }
}
void MainWindow::setupMonitorTab()
{
    // Set the behavior of the tree view
    ui->monitorTree->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->monitorTree->setIndentation(0);
    // Add the items
    gputemp->setText(0, "Core Temperature");
    powerdraw->setText(0, "Power Draw");
    voltage->setText(0, "Core Voltage");
    coreclock->setText(0, "Core Clock Frequency");
    memclock->setText(0, "Memory Clock Frequency");
    coreutil->setText(0, "Core Utilization");
    memutil->setText(0, "Memory Utilization");
    fanspeed->setText(0, "Fan Speed");
    memusage->setText(0, "Used Memory/Total Memory");
    curmaxclk->setText(0, "Maximum Core Clock Frequency");
    curmaxmemclk->setText(0, "Maximum Memory Clock Frequency");

    ui->monitorTree->addTopLevelItem(gputemp);
    ui->monitorTree->addTopLevelItem(powerdraw);
    ui->monitorTree->addTopLevelItem(voltage);
    ui->monitorTree->addTopLevelItem(coreclock);
    ui->monitorTree->addTopLevelItem(memclock);
    ui->monitorTree->addTopLevelItem(coreutil);
    ui->monitorTree->addTopLevelItem(memutil);
    ui->monitorTree->addTopLevelItem(fanspeed);
    ui->monitorTree->addTopLevelItem(memusage);
    ui->monitorTree->addTopLevelItem(curmaxclk);
    ui->monitorTree->addTopLevelItem(curmaxmemclk);
    // These values only change when the apply button has been pressed
    QString curMaxClk = QString::number(types->GPUList[currentGPUIndex].maxCoreClk) + " MHz";
    curmaxclk->setText(1, curMaxClk);
    QString curMaxMemClk = QString::number(types->GPUList[currentGPUIndex].maxMemClk) + " MHz";
    curmaxmemclk->setText(1, curMaxMemClk);
}
void MainWindow::setupGraphMonitorTab()
{
    types->queryGPUTemp(currentGPUIndex);
    types->queryGPUPowerDraw(currentGPUIndex);
    types->queryGPUFrequencies(currentGPUIndex);
    types->queryGPUUtils(currentGPUIndex);
    types->queryGPUFanSpeed(currentGPUIndex);
    types->queryGPUVoltage(currentGPUIndex);

    plotCmdsList.append(powerdrawplot);
    plotCmdsList.append(tempplot);
    plotCmdsList.append(coreclkplot);
    plotCmdsList.append(memclkplot);
    plotCmdsList.append(coreutilplot);
    plotCmdsList.append(memutilplot);
    plotCmdsList.append(voltageplot);
    plotCmdsList.append(fanspeedplot);
    // Layout for the plots
    plotWidget->setLayout(plotLayout);

    // Define the structs
    plotCmdsList[0].plot = tempPlot;
    plotCmdsList[1].plot = powerDrawPlot;
    plotCmdsList[2].plot = coreClkPlot;
    plotCmdsList[3].plot = memClkPlot;
    plotCmdsList[4].plot = coreUtilPlot;
    plotCmdsList[5].plot = memUtilPlot;
    plotCmdsList[6].plot = voltagePlot;
    plotCmdsList[7].plot = fanSpeedPlot;

    /*plotCmdsList[0].valueq = types->GPUList[currentGPUIndex].temp;
    plotCmdsList[1].valueq = types->GPUList[currentGPUIndex].powerDraw/1000;
    plotCmdsList[2].valueq = types->GPUList[currentGPUIndex].coreFreq;
    plotCmdsList[3].valueq = types->GPUList[currentGPUIndex].memFreq;
    plotCmdsList[4].valueq = types->GPUList[currentGPUIndex].coreUtil;
    plotCmdsList[5].valueq = types->GPUList[currentGPUIndex].memUtil;
    plotCmdsList[6].valueq = types->GPUList[currentGPUIndex].voltage/1000;
    plotCmdsList[7].valueq = types->GPUList[currentGPUIndex].fanSpeed;*/

    types->calculateDisplayValues(currentGPUIndex);
    plotCmdsList[0].valueq = types->GPUList[currentGPUIndex].displayTemp;
    plotCmdsList[1].valueq = types->GPUList[currentGPUIndex].displayPowerDraw;
    plotCmdsList[2].valueq = types->GPUList[currentGPUIndex].displayCoreFreq;
    plotCmdsList[3].valueq = types->GPUList[currentGPUIndex].displayMemFreq;
    plotCmdsList[4].valueq = types->GPUList[currentGPUIndex].displayCoreUtil;
    plotCmdsList[5].valueq = types->GPUList[currentGPUIndex].displayMemUtil;
    plotCmdsList[6].valueq = types->GPUList[currentGPUIndex].displayVoltage;
    plotCmdsList[7].valueq = types->GPUList[currentGPUIndex].displayFanSpeed;


    // Get the main widget palette and use it for the graphs
    QPalette palette;
    palette.setCurrentColorGroup(QPalette::Active);
    QColor color = palette.color(QPalette::Background);
    QColor textColor = palette.color(QPalette::Text);
    QColor graphColor = palette.color(QPalette::Highlight);
    QPen graphPen;
    graphPen.setWidth(2);
    graphPen.setColor(graphColor);
    QPen tickPen;
    tickPen.setWidthF(0.5);
    tickPen.setColor(textColor);
    QPen tracerPen = graphPen;
    tracerPen.setWidthF(0.5);

    // Button to reset extreme values
    QPushButton *resetButton = new QPushButton(this);
    resetButton->setMaximumWidth(150);
    resetButton->setText("Reset min/max");
    connect(resetButton, SIGNAL(clicked()), SLOT(clearExtremeValues()));
    plotLayout->addWidget(resetButton);
    // Create data vectors for the GPUs
    for (int i=0; i<types->gpuCount; i++) {
        GPUData gpu;
        for (int j=0; j<plotCmdsList.size(); j++) {
            datavector vec;
            gpu.data.append(vec);
        }
        GPU.append(gpu);
    }
    qDebug() << GPU.size() << "GPU vec amount";
    // Define features common to all plots
    for (int i=0; i<plotCmdsList.size(); i++) {
        //QVector <double> vector;
        //GPU[currentGPUIndex].data[i].vector = vector;

        plotCmdsList[i].plot->setMinimumHeight(220);
        plotCmdsList[i].plot->setMaximumHeight(220);
        plotCmdsList[i].plot->setMinimumWidth(200);

        plotCmdsList[i].plot->addGraph();
        plotCmdsList[i].plot->xAxis->setRange(-plotVectorSize +1, 0);
        plotCmdsList[i].plot->xAxis->setLabel("Time (s)");
        plotCmdsList[i].plot->installEventFilter(this);

        // Add the widget to the main layout
        QWidget *widget = new PlotWidget(this);
        plotCmdsList[i].widget = widget;
        connect(plotCmdsList[i].widget, SIGNAL(leftPlot()), SLOT(clearPlots()));

        QVBoxLayout *layout = new QVBoxLayout;
        plotCmdsList[i].layout = layout;

        plotCmdsList[i].widget->setLayout(plotCmdsList[i].layout);
        plotCmdsList[i].layout->addWidget(plotCmdsList[i].plot);
        plotLayout->addWidget(plotCmdsList[i].widget);

        plotCmdsList[i].plot->setBackground(color);
        plotCmdsList[i].plot->xAxis->setLabelColor(textColor);
        plotCmdsList[i].plot->yAxis->setLabelColor(textColor);
        plotCmdsList[i].plot->xAxis->setTickLabelColor(textColor);
        plotCmdsList[i].plot->yAxis->setTickLabelColor(textColor);
        plotCmdsList[i].plot->graph(0)->setPen(graphPen);
        plotCmdsList[i].plot->xAxis->setTickPen(tickPen);
        plotCmdsList[i].plot->yAxis->setTickPen(tickPen);
        plotCmdsList[i].plot->xAxis->setSubTickPen(tickPen);
        plotCmdsList[i].plot->yAxis->setSubTickPen(tickPen);
        plotCmdsList[i].plot->xAxis->setBasePen(tickPen);
        plotCmdsList[i].plot->yAxis->setBasePen(tickPen);

        QCPTextElement *minelem = new QCPTextElement(plotCmdsList[i].plot);
        plotCmdsList[i].mintext = minelem;
        minelem->setText("Min: " + QString::number(plotCmdsList[i].valueq));
        minelem->setTextColor(textColor);

        QCPTextElement *maxelem = new QCPTextElement(plotCmdsList[i].plot);
        plotCmdsList[i].maxtext = maxelem;
        maxelem->setText("Max: " + QString::number(plotCmdsList[i].valueq));
        maxelem->setTextColor(textColor);

        QCPTextElement *curelem = new QCPTextElement(plotCmdsList[i].plot);
        plotCmdsList[i].curtext = curelem;
        curelem->setText("Cur: " + QString::number(plotCmdsList[i].valueq));
        curelem->setTextColor(textColor);

        plotCmdsList[i].plot->plotLayout()->insertRow(0);
        QCPLayoutGrid *sublo = new QCPLayoutGrid;
        plotCmdsList[i].plot->plotLayout()->addElement(0, 0, sublo);
        sublo->setMargins(QMargins(5 ,5, 5, 5));
        sublo->addElement(plotCmdsList[i].mintext);
        sublo->addElement(plotCmdsList[i].maxtext);
        sublo->addElement(plotCmdsList[i].curtext);

        QCPItemText *text = new QCPItemText(plotCmdsList[i].plot);
        text->setColor(textColor);
        plotCmdsList[i].valText = text;

        // Set the y-range
        plotCmdsList[i].plot->yAxis->setRange(plotCmdsList[i].valueq -plotCmdsList[i].valueq*0.1, plotCmdsList[i].valueq + plotCmdsList[i].valueq*0.1);

        // Add the tracers
        QCPItemTracer *mouseTracer = new QCPItemTracer(plotCmdsList[i].plot);
        plotCmdsList[i].tracer = mouseTracer;
        mouseTracer->setStyle(QCPItemTracer::tsCrosshair);
        mouseTracer->setPen(tracerPen);
        connect(plotCmdsList[i].plot, SIGNAL(mouseMove(QMouseEvent*)), SLOT(plotHovered(QMouseEvent*)));

        plotCmdsList[i].maxval = plotCmdsList[i].valueq;
        plotCmdsList[i].minval = plotCmdsList[i].valueq;
    }
    // Hide the plots for values that can't be read
    if (!types->GPUList[currentGPUIndex].coreClkReadable) {
        plotCmdsList[2].widget->setVisible(false);
    }
    if (!types->GPUList[currentGPUIndex].memClkReadable) {
        plotCmdsList[3].widget->setVisible(false);
    }
    if (!types->GPUList[currentGPUIndex].voltageReadable) {
        plotCmdsList[6].widget->setVisible(false);
    }

    tempPlot->yAxis->setLabel("Temperature (°C)");
    powerDrawPlot->yAxis->setLabel("Power Draw (W)");
    coreClkPlot->yAxis->setLabel("Core Clock Frequency (MHz)");
    memClkPlot->yAxis->setLabel("Memory Clock Frequency (MHz)");
    coreUtilPlot->yAxis->setLabel("Core Utilization (%)");
    memUtilPlot->yAxis->setLabel("Memory Utilization (%)");
    voltagePlot->yAxis->setLabel("Core Voltage (mV)");
    fanSpeedPlot->yAxis->setLabel("Fan Speed (%)");

    plotScrollArea->setWidget(plotWidget);
    plotScrollArea->setWidgetResizable(true);

    // Add scroll area to a layout so we can set it as the widget for the tab
    lo->addWidget(plotScrollArea);
    ui->monitorTab->setLayout(lo);

    //connect(plotHoverUpdater, SIGNAL(timeout()), SLOT(plotHovered()));
}
void MainWindow::updateMonitor()
{
    // Update the values for plots
    types->queryGPUTemp(currentGPUIndex);
    types->queryGPUPowerDraw(currentGPUIndex);
    types->queryGPUFrequencies(currentGPUIndex);
    types->queryGPUUtils(currentGPUIndex);
    types->queryGPUVoltage(currentGPUIndex);
    types->queryGPUFanSpeed(currentGPUIndex);
    types->queryGPUUsedVRAM(currentGPUIndex);

    // Remove the last decimal point from power draw to make it take less space on the plot
    /*double pwrdraw = types->GPUList[currentGPUIndex].powerDraw;
    pwrdraw = pwrdraw/10;
    int num = static_cast<int>(pwrdraw);
    pwrdraw = static_cast<double>(num);
    pwrdraw = pwrdraw/100;*/

    /*plotCmdsList[0].valueq = types->GPUList[currentGPUIndex].temp;
    plotCmdsList[1].valueq = pwrdraw;
    plotCmdsList[2].valueq = types->GPUList[currentGPUIndex].coreFreq;
    plotCmdsList[3].valueq = types->GPUList[currentGPUIndex].memFreq;
    plotCmdsList[4].valueq = types->GPUList[currentGPUIndex].coreUtil;
    plotCmdsList[5].valueq = types->GPUList[currentGPUIndex].memUtil;
    plotCmdsList[6].valueq = types->GPUList[currentGPUIndex].voltage/1000;
    plotCmdsList[7].valueq = types->GPUList[currentGPUIndex].fanSpeed;*/

    types->calculateDisplayValues(currentGPUIndex);
    plotCmdsList[0].valueq = types->GPUList[currentGPUIndex].displayTemp;
    plotCmdsList[1].valueq = types->GPUList[currentGPUIndex].displayPowerDraw;
    plotCmdsList[2].valueq = types->GPUList[currentGPUIndex].displayCoreFreq;
    plotCmdsList[3].valueq = types->GPUList[currentGPUIndex].displayMemFreq;
    plotCmdsList[4].valueq = types->GPUList[currentGPUIndex].displayCoreUtil;
    plotCmdsList[5].valueq = types->GPUList[currentGPUIndex].displayMemUtil;
    plotCmdsList[6].valueq = types->GPUList[currentGPUIndex].displayVoltage;
    plotCmdsList[7].valueq = types->GPUList[currentGPUIndex].displayFanSpeed;

    qDebug() << monitorUpdater->remainingTime();

    gputemp->setText(1, QString::number(types->GPUList[currentGPUIndex].temp) + "°C");
    powerdraw->setText(1, QString::number(types->GPUList[currentGPUIndex].powerDraw/1000) + "W");
    voltage->setText(1, QString::number(types->GPUList[currentGPUIndex].voltage/1000) + "mV");
    coreclock->setText(1, QString::number(types->GPUList[currentGPUIndex].coreFreq) + "MHz");
    memclock->setText(1, QString::number(types->GPUList[currentGPUIndex].memFreq) + "MHz");
    coreutil->setText(1, QString::number(types->GPUList[currentGPUIndex].coreUtil) + "%");
    memutil->setText(1, QString::number(types->GPUList[currentGPUIndex].memUtil) + "%");
    fanspeed->setText(1, QString::number(types->GPUList[currentGPUIndex].fanSpeed) + "%");
    memusage->setText(1, QString::number(types->GPUList[currentGPUIndex].usedVRAM) + "/" + QString::number(types->GPUList[currentGPUIndex].totalVRAM) + "MB");

    // Decrement all time values by one
    for (int i=0; i<GPU[currentGPUIndex].qv_time.length(); i++) {
        GPU[currentGPUIndex].qv_time[i]--;
    }
    // Add current time (0)
    if (GPU[currentGPUIndex].qv_time.size() < plotVectorSize) {
        GPU[currentGPUIndex].qv_time.append(0);
    } else {
        GPU[currentGPUIndex].qv_time.insert(plotVectorSize, 0);
    }
    // Remove the first elements if there are more elements than the x-range
    if (GPU[currentGPUIndex].qv_time.size() > plotVectorSize) {
        GPU[currentGPUIndex].qv_time.removeFirst();
    }

    for (int i=0; i<plotCmdsList.size(); i++) {
        // Check if the max/min values need to be updated
        if (!GPU[currentGPUIndex].data[i].vector.isEmpty()) {
            plotCmdsList[i].curtext->setText("Cur: " + QString::number(plotCmdsList[i].valueq));
            if (plotCmdsList[i].maxval < plotCmdsList[i].valueq) {
                plotCmdsList[i].maxtext->setText("Max: " + QString::number(plotCmdsList[i].valueq));
                plotCmdsList[i].maxval = plotCmdsList[i].valueq;
            }
            if (plotCmdsList[i].minval > plotCmdsList[i].valueq) {
                plotCmdsList[i].mintext->setText("Min: " + QString::number(plotCmdsList[i].valueq));
                plotCmdsList[i].minval = plotCmdsList[i].valueq;
            }
        }
        if (GPU[currentGPUIndex].data[i].vector.size() < plotVectorSize) {
            GPU[currentGPUIndex].data[i].vector.append(plotCmdsList[i].valueq);
        } else {
            GPU[currentGPUIndex].data[i].vector.insert(plotVectorSize, plotCmdsList[i].valueq);
        }
        // Remove the first element if there are more elements than the x-range
        if (GPU[currentGPUIndex].data[i].vector.size() > plotVectorSize) {
            GPU[currentGPUIndex].data[i].vector.removeFirst();
        }
        plotCmdsList[i].plot->graph(0)->setData(GPU[currentGPUIndex].qv_time, GPU[currentGPUIndex].data[i].vector);
        // If the newest value is out of bounds, resize the y-range
        if (plotCmdsList[i].valueq > plotCmdsList[i].plot->yAxis->range().upper) {
            plotCmdsList[i].plot->yAxis->setRangeUpper(plotCmdsList[i].valueq + plotCmdsList[i].valueq*0.1);
        }
        if (plotCmdsList[i].valueq < plotCmdsList[i].plot->yAxis->range().lower) {
            plotCmdsList[i].plot->yAxis->setRangeLower(plotCmdsList[i].valueq - plotCmdsList[i].valueq*0.1);
        }
        plotCmdsList[i].plot->replot();
        plotCmdsList[i].plot->update();
    }
    //qDebug() << monitorUpdater->remainingTime();
    // If the largest/smallest value is too far from the range end, this resizes them every 10th iteration of this function
    if (counter >= 10) {
        for (int i=0; i<plotCmdsList.size(); i++) {
            double lowestval = GPU[currentGPUIndex].data[i].vector[0];
            double largestval = GPU[currentGPUIndex].data[i].vector[0];
            for (int j=0; j<GPU[currentGPUIndex].data[i].vector.size(); j++) {
                if (GPU[currentGPUIndex].data[i].vector[j] < lowestval) {
                    lowestval = GPU[currentGPUIndex].data[i].vector[j];
                }
                if (GPU[currentGPUIndex].data[i].vector[j] > largestval) {
                    largestval = GPU[currentGPUIndex].data[i].vector[j];
                }
            }
            if (plotCmdsList[i].plot->yAxis->range().upper - largestval*0.1 > largestval) {
                plotCmdsList[i].plot->yAxis->setRange(lowestval - lowestval*0.1, largestval + largestval*0.1);
            }
            if (plotCmdsList[i].plot->yAxis->range().lower + lowestval*0.1 < lowestval) {
                // Don't set the lower range to under 0
                if (lowestval - lowestval*0.1 < 0) {
                    plotCmdsList[i].plot->yAxis->setRangeLower(0);
                } else {
                    plotCmdsList[i].plot->yAxis->setRange(lowestval - lowestval*0.1, largestval + largestval*0.1);
                }
            }
        }
        counter = 0;
    }
    counter++;
    if (insidePlot) updateTracer();
}
void MainWindow::plotHovered(QMouseEvent *event)
{
    QPoint cursor = event->pos();
    plotIndex = 0;
    for (int i=0; i<plotCmdsList.size(); i++) {
        if (plotCmdsList[i].widget->underMouse()) {
            plotIndex = i;
            break;
        }
    }
    insidePlot = true;
    double pointerxcoord = plotCmdsList[plotIndex].plot->xAxis->pixelToCoord(cursor.x());
    latestPointerXcoord = pointerxcoord;

    plotCmdsList[plotIndex].tracer->position->setCoords(pointerxcoord, plotCmdsList[plotIndex].plot->yAxis->range().upper);
    // Find the y-value for the corresponding coordinate
    int valIndex = 0;
    if (!GPU[currentGPUIndex].qv_time.isEmpty() && pointerxcoord > -plotVectorSize*1.01 && pointerxcoord <= 0 + plotVectorSize*0.01) {
        double deltax = abs(GPU[currentGPUIndex].qv_time[0] - pointerxcoord);
        for (int i=0; i<GPU[currentGPUIndex].qv_time.size(); i++) {
            if (abs(GPU[currentGPUIndex].qv_time[i] - pointerxcoord) < deltax) {
                deltax = abs(GPU[currentGPUIndex].qv_time[i] - pointerxcoord);
                valIndex = i;
            }
        }
        plotCmdsList[plotIndex].valText->setText(QString::number(GPU[currentGPUIndex].data[plotIndex].vector[valIndex]));
        // Make the text stay inside the plot
        if (pointerxcoord > -plotVectorSize*0.06) {
            plotCmdsList[plotIndex].valText->position->setCoords(-plotVectorSize*0.06, plotCmdsList[plotIndex].plot->yAxis->range().upper - plotCmdsList[plotIndex].plot->yAxis->range().size()*0.05);
        } else if (pointerxcoord < -plotVectorSize*0.94) {
            plotCmdsList[plotIndex].valText->position->setCoords(-plotVectorSize*0.94, plotCmdsList[plotIndex].plot->yAxis->range().upper - plotCmdsList[plotIndex].plot->yAxis->range().size()*0.05);
        } else {
            plotCmdsList[plotIndex].valText->position->setCoords(pointerxcoord, plotCmdsList[plotIndex].plot->yAxis->range().upper - plotCmdsList[plotIndex].plot->yAxis->range().size()*0.05);
        }
        QThread::msleep(10);
    } else {
        // If the cursor is not within the x-range, clear the text
        plotCmdsList[plotIndex].valText->setText("");
        insidePlot = false;
    }
    plotCmdsList[plotIndex].plot->update();
    plotCmdsList[plotIndex].plot->replot();
}
void MainWindow::updateTracer()
{
    int index = 0;
    double delta = abs(latestPointerXcoord - GPU[currentGPUIndex].qv_time[0]);
    for (int i=0; i<GPU[currentGPUIndex].qv_time.size(); i++) {
        if (abs(latestPointerXcoord - GPU[currentGPUIndex].qv_time[i]) < delta) {
            delta = abs(latestPointerXcoord - GPU[currentGPUIndex].qv_time[i]);
            index = i;
        }
    }
    plotCmdsList[plotIndex].valText->setText(QString::number(GPU[currentGPUIndex].data[plotIndex].vector[index]));
}
void MainWindow::clearPlots()
{
    for (int i=0; i<plotCmdsList.size(); i++) {
        plotCmdsList[i].valText->setText("");
        plotCmdsList[i].tracer->position->setCoords(1, -1);
        plotCmdsList[i].plot->replot();
        plotCmdsList[i].plot->update();
    }
    insidePlot = false;
}
void MainWindow::clearExtremeValues()
{
    for (int i=0; i<plotCmdsList.size(); i++) {
        plotCmdsList[i].minval = plotCmdsList[i].valueq;
        plotCmdsList[i].maxval = plotCmdsList[i].valueq;
        plotCmdsList[i].mintext->setText("Min: " + QString::number(plotCmdsList[i].valueq));
        plotCmdsList[i].maxtext->setText("Max: " + QString::number(plotCmdsList[i].valueq));
    }
}
void MainWindow::checkForProfiles()
{
    // If there are no profiles, create one, then list all the entries whose isProfile is true in the profile selection combo box
    QSettings settings("tuxclocker");
    QStringList groups = settings.childGroups();
    QString isProfile = "/isProfile";

    for (int i=0; i<groups.length(); i++) {
        // Make a query $profile/isProfile
        QString group = groups[i];
        QString query = group.append(isProfile);
        if (settings.value(query).toBool()) {
            noProfiles = false;
            break;
        }
    }
    if (noProfiles) {
        settings.setValue("New Profile/isProfile", true);
        settings.setValue("General/currentProfile", "New Profile");
        currentProfile = "New Profile";
    }
    // Redefine child groups so it contains the newly created profile if it was made
    QStringList newgroups = settings.childGroups();
    for (int i=0; i<newgroups.length(); i++) {
        // Make a query $profile/isProfile
        QString group = newgroups[i];
        QString query = group.append(isProfile);
        if (settings.value(query).toBool()) {
            ui->profileComboBox->addItem(newgroups[i]);
        }
    }
}

void MainWindow::on_profileComboBox_activated(const QString &arg1)
{
    // Change currentProfile to combobox selection
    if (currentProfile != arg1) {
        currentProfile = arg1;
        QSettings settings("tuxclocker");
        settings.setValue("General/currentProfile", currentProfile);
        loadProfileSettings();
    }
    resettimer->stop();
}

void MainWindow::getGPUDriver()
{
    QProcess process;
    process.start(queryForNvidiaProp);
    process.waitForFinished(-1);
    if (process.readAllStandardOutput().toInt() >= 1) {
        gpuDriver = "nvidia";
    }
}
void MainWindow::fanSpeedUpdater()
{
    types->queryGPUFanSpeed(currentGPUIndex);
    ui->fanSlider->setValue(types->GPUList[currentGPUIndex].fanSpeed);
    ui->fanSpinBox->setValue(types->GPUList[currentGPUIndex].fanSpeed);
}
void MainWindow::tempUpdater()
{
    types->queryGPUTemp(currentGPUIndex);
    qDebug() << "updating temp";
    qDebug() << xCurvePoints << yCurvePoints;
    if (xCurvePoints.size() != 0) {
        generateFanPoint();
    }
}
void MainWindow::resetTimer()
{
    // If a value has been changed this timer will start. When the apply button has been pressed, this gets cancelled
    connect(resettimer, SIGNAL(timeout()), SLOT(resetChanges()));
    resettimer->stop();
    resettimer->setSingleShot(true);
    resettimer->start(10000);
}
void MainWindow::resetChanges()
{
    // If the settings haven't been applied in 10 seconds, reset all values to their latest values
    ui->frequencySlider->setValue(types->GPUList[currentGPUIndex].coreClkOffset);
    ui->frequencySpinBox->setValue(types->GPUList[currentGPUIndex].coreClkOffset);

    ui->powerLimSlider->setValue(types->GPUList[currentGPUIndex].powerLim/1000);
    ui->powerLimSpinBox->setValue(types->GPUList[currentGPUIndex].powerLim/1000);

    ui->voltageSlider->setValue(types->GPUList[currentGPUIndex].voltageOffset/1000);
    ui->voltageSpinBox->setValue(types->GPUList[currentGPUIndex].voltageOffset/1000);

    ui->memClkSlider->setValue(types->GPUList[currentGPUIndex].memClkOffset/2);
    ui->memClkSpinBox->setValue(types->GPUList[currentGPUIndex].memClkOffset/2);
}
void MainWindow::applyGPUSettings()
{
    ui->statusBar->showMessage(types->applySettings(currentGPUIndex), 5000);
    /*
    // Apply and save the values
    QSettings settings("tuxclocker");
    settings.beginGroup("General");
    settings.setValue("latestUUID", types->GPUList[currentGPUIndex].uuid);
    qDebug() << currentProfile << "curprf";
    settings.endGroup();
    settings.beginGroup(currentProfile);
    settings.beginGroup(types->GPUList[currentGPUIndex].uuid);
    QProcess process;
    int offsetValue;
    int powerLimit;
    bool hadErrors = false;
    bool ret;
    errorText = "Failed to apply these settings: ";
    QString input = nvCoreClkSet;
    if (types->GPUList[currentGPUIndex].coreClkOffset != ui->frequencySlider->value() && types->GPUList[currentGPUIndex].overClockAvailable) {
        offsetValue = ui->frequencySlider->value();
        ret = types->assignGPUFreqOffset(currentGPUIndex, offsetValue);
        if (ret) {
            types->GPUList[currentGPUIndex].coreClkOffset = offsetValue;
            settings.setValue("clockFrequencyOffset", offsetValue);
        } else {
            errorText.append("- Core Clock Offset");
            ui->frequencySlider->setValue(types->GPUList[currentGPUIndex].coreClkOffset);
            hadErrors = true;
        }
    }

    if (types->GPUList[currentGPUIndex].memFreq/2 != ui->memClkSlider->value() && types->GPUList[currentGPUIndex].overClockAvailable) {
        offsetValue = ui->memClkSlider->value();
        ret = types->assignGPUMemClockOffset(currentGPUIndex, offsetValue*2);
        if (ret) {
            types->GPUList[currentGPUIndex].memClkOffset = offsetValue*2;
            settings.setValue("memoryClockOffset", offsetValue*2);
        } else {
            errorText.append("- Memory Clock Offset");
            hadErrors = true;
            ui->frequencySlider->setValue(types->GPUList[currentGPUIndex].memClkOffset/2);
        }
    }

    if (types->GPUList[currentGPUIndex].powerLim/1000 != ui->powerLimSlider->value() && types->GPUList[currentGPUIndex].powerLimitAvailable) {
        powerLimit = ui->powerLimSlider->value();
        if (!isRoot) {
            // If user is not root, call nvidia-smi through pkexec until I come up with a cleaner solution
            //ret = types->assignGPUPowerLimit(powerLimit*1000);
            qDebug() << "trying pkexec";
            QProcess prc;
            prc.start("/bin/sh -c \"pkexec " + nvPowerLimSet + QString::number(powerLimit) + " -i " + QString::number(currentGPUIndex) + "\"");
            prc.waitForFinished();
            if (prc.exitCode() == 0) {
                types->GPUList[currentGPUIndex].powerLim = powerLimit*1000;
                settings.setValue("powerLimit", powerLimit);
            } else {
                errorText.append("- Power Limit");
                hadErrors = true;
                ui->powerLimSlider->setValue(types->GPUList[currentGPUIndex].powerLim/1000);
            }
        } else {
            ret = types->assignGPUPowerLimit(powerLimit*1000);
            if (ret) {
                types->GPUList[currentGPUIndex].powerLim = powerLimit*1000;
                settings.setValue("powerLimit", powerLimit);
            } else {
                errorText.append("- Power Limit");
                hadErrors = true;
                ui->powerLimSlider->setValue(types->GPUList[currentGPUIndex].powerLim/1000);
            }
        }
    }

    if (latestVoltOfs != ui->voltageSlider->value() && types->GPUList[currentGPUIndex].overVoltAvailable) {
        offsetValue = ui->voltageSlider->value();
        ret = types->assignGPUVoltageOffset(currentGPUIndex, offsetValue*1000);
        if (ret) {
            types->GPUList[currentGPUIndex].voltageOffset = offsetValue*1000;
                settings.setValue("voltageOffset", offsetValue*1000);
        } else {
            errorText.append("- Voltage Offset");
            hadErrors = true;
            ui->voltageSlider->setValue(types->GPUList[currentGPUIndex].voltageOffset/1000);
        }
    }
    // Apply fan mode
    qDebug() << "changing fanctl to" << types->GPUList[currentGPUIndex].fanControlMode;
    switch (types->GPUList[currentGPUIndex].fanControlMode) {
        case 0:
            // Driver controlled mode
            ret = types->assignGPUFanCtlMode(currentGPUIndex, false);
            if (ret) {
                ui->fanSlider->setEnabled(false);
                ui->fanSpinBox->setEnabled(false);
                settings.setValue("fanControlMode", 0);
            } else {
                errorText.append("- Fan mode");
            }
            break;
        case 1:
           // Static mode
            ret = types->assignGPUFanCtlMode(currentGPUIndex, true);
            if (ret) {
                disconnect(fanUpdateTimer, SIGNAL(timeout()), this, SLOT(tempUpdater()));
                types->assignGPUFanSpeed(currentGPUIndex, ui->fanSlider->value());
                ui->fanSlider->setEnabled(true);
                ui->fanSpinBox->setEnabled(true);
                settings.setValue("fanControlMode", 1);
            } else {
                errorText.append("- Fan mode");
            }
            break;
        case 2:
            // Custom mode
            ret = types->assignGPUFanCtlMode(currentGPUIndex, true);
            if (ret) {
                connect(fanUpdateTimer, SIGNAL(timeout()), this, SLOT(tempUpdater()));
                ui->fanSlider->setEnabled(false);
                ui->fanSpinBox->setEnabled(false);
                fanUpdateTimer->start(2000);
                settings.setValue("fanControlMode", 2);
            } else {
                errorText.append("- Fan mode");
            }
            break;
    }

    if (hadErrors) {
        ui->statusBar->showMessage(errorText, 5000);
    } else {
        ui->statusBar->showMessage("Settings applied", 5000);
    }
    resettimer->stop();*/
}

void MainWindow::loadProfileSettings()
{
    QSettings settings("tuxclocker");
    currentProfile = settings.value("General/currentProfile").toString();
    latestUUID = settings.value("General/latestUUID").toString();
    // Set the profile combo box selection to currentProfile
    for (int i=0; i<ui->profileComboBox->count(); i++) {
        if (ui->profileComboBox->itemText(i).contains(currentProfile)) {
            ui->profileComboBox->setCurrentIndex(i);
            break;
        }
    }
    settings.beginGroup(currentProfile);
    settings.beginGroup(latestUUID);
    // Check if manual control is available and set the combo box accordingly
    if (types->GPUList[currentGPUIndex].manualFanCtrlAvailable) {
        xCurvePoints.clear();
        yCurvePoints.clear();
        int size = settings.beginReadArray("curvepoints");
        for (int i=0; i<size; i++) {
            settings.setArrayIndex(i);
            xCurvePoints.append(settings.value("xpoints").toInt());
            yCurvePoints.append(settings.value("ypoints").toInt());
        }
        settings.endArray();
        QStandardItemModel *model = qobject_cast<QStandardItemModel*>(ui->fanModeComboBox->model());
        QModelIndex customModeIndex = model->index(2, ui->fanModeComboBox->modelColumn());
        QStandardItem *customMode = model->itemFromIndex(customModeIndex);
        customMode->setEnabled(true);
        customMode->setToolTip("Use your own fan curve");
    } else {
        // Set fan mode "Custom" unselectable if there are no custom curve points
        QStandardItemModel *model = qobject_cast<QStandardItemModel*>(ui->fanModeComboBox->model());
        QModelIndex customModeIndex = model->index(2, ui->fanModeComboBox->modelColumn());
        QStandardItem *customMode = model->itemFromIndex(customModeIndex);
        customMode->setEnabled(false);
        customMode->setToolTip("To use this mode you must make a fan curve first");
    }

    if (settings.contains("voltageOffset")) {
        latestVoltOfs = settings.value("voltageOffset").toInt();

        ui->voltageSlider->setValue(latestVoltOfs);
        ui->voltageSpinBox->setValue(latestVoltOfs);
    }
    if (settings.contains("powerLimit")) {
        latestPowerLim = settings.value("powerLimit").toInt();

        ui->powerLimSlider->setValue(latestPowerLim);
        ui->powerLimSlider->setValue(latestPowerLim);
    }
    if (settings.contains("clockFrequencyOffset")) {
        latestClkOfs = settings.value("clockFrequencyOffset").toInt();

        ui->frequencySlider->setValue(latestClkOfs);
        ui->frequencySpinBox->setValue(latestClkOfs);
    }
    if (settings.contains("memoryClockOffset")) {
        latestMemClkOfs=settings.value("memoryClockOffset").toInt();
        ui->memClkSlider->setValue(latestMemClkOfs);
        ui->memClkSlider->setValue(latestMemClkOfs);
        qDebug() << latestMemClkOfs << "is now memclkoffset";
    }
    if (settings.contains("fanControlMode")) {
        fanControlMode = settings.value("fanControlMode").toInt();
        ui->fanModeComboBox->setCurrentIndex(fanControlMode);
        if (fanControlMode != 1) {
            ui->fanSlider->setEnabled(false);
            ui->fanSpinBox->setEnabled(false);
        } else {
            ui->fanSlider->setEnabled(true);
            ui->fanSpinBox->setEnabled(true);
        }
    }
    // Check which GPU index corresponds to the UUID and set the combo box selection to it
    for (int i=0; i<types->gpuCount; i++) {
        if (types->GPUList[i].uuid == latestUUID) {
            ui->GPUComboBox->setCurrentIndex(i);
            break;
        }
    }
    ui->statusBar->showMessage("Profile settings loaded.", 7000);
}

void MainWindow::on_newProfile_closed()
{
    // If currentProfile doesn't exist anymore the first profile will be the first entry
    QSettings settings("tuxclocker");
    QStringList groups = settings.childGroups();
    if (!groups.contains(currentProfile)) {
        for (int i=0; i<groups.size(); i++) {
            settings.beginGroup(groups[i]);
            if (settings.value("isProfile").toBool()) {
                settings.endGroup();
                currentProfile = groups[i];
                break;
            }
            settings.endGroup();
        }
    }
    //settings.endGroup();
    settings.setValue("General/currentProfile", currentProfile);
    ui->profileComboBox->clear();
    //ui->GPUComboBox->clear();
    checkForProfiles();
    loadProfileSettings();
}
void MainWindow::saveProfileSettings()
{
    QSettings settings("tuxclocker");
    settings.beginGroup("General");
    settings.setValue("latestUUID", types->GPUList[currentGPUIndex].uuid);
    settings.endGroup();
    qDebug() << "current prf" << currentProfile;
    settings.beginGroup(currentProfile);
    settings.beginGroup(types->GPUList[currentGPUIndex].uuid);
    settings.setValue("voltageOffset", types->GPUList[currentGPUIndex].voltageOffset);
    settings.setValue("powerLimit", latestPowerLim);
    settings.setValue("clockFrequencyOffset", latestClkOfs);
    settings.setValue("memoryClockOffset", latestMemClkOfs);
    settings.setValue("fanControlMode", fanControlMode);
}

void MainWindow::generateFanPoint()
{
    // Calculate the value for fan speed based on temperature
    // First check if the fan speed should be y[0] or y[final]
    int index = 0;
    if (types->GPUList[currentGPUIndex].temp <= xCurvePoints[0]) {
        targetFanSpeed = yCurvePoints[0];
    }
    else if (types->GPUList[currentGPUIndex].temp  >= xCurvePoints[xCurvePoints.size()-1]) {
        targetFanSpeed = yCurvePoints[yCurvePoints.size()-1];
    } else {
        // Get the index of the leftmost point of the interpolated interval by comparing it to temperature
        for (int i=0; i<xCurvePoints.size(); i++) {
            if (types->GPUList[currentGPUIndex].temp  >= xCurvePoints[i] && types->GPUList[currentGPUIndex].temp <= xCurvePoints[i+1]) {
                index = i;
                break;
            }
        }
        // Check if the change in x is zero to avoid dividing by it
        if (xCurvePoints[index] - xCurvePoints[index + 1] == 0) {
            targetFanSpeed = yCurvePoints[index+1];
        } else {
            targetFanSpeed = (((yCurvePoints[index + 1] - yCurvePoints[index]) * (types->GPUList[currentGPUIndex].temp  - xCurvePoints[index])) / (xCurvePoints[index + 1] - xCurvePoints[index])) + yCurvePoints[index];
        }
    }
    qDebug() << "target fan speed is" << targetFanSpeed;
    types->assignGPUFanSpeed(currentGPUIndex, targetFanSpeed);
}
void MainWindow::on_frequencySlider_valueChanged(int value)
{
    // Sets the input field value to slider value
    QString freqText = QString::number(value);
    ui->frequencySpinBox->setValue(value);
}

void MainWindow::on_frequencySpinBox_valueChanged(int arg1)
{
    ui->frequencySlider->setValue(arg1);
}

void MainWindow::on_newProfile_clicked()
{
    newProfile *newprof = new newProfile(this);
    newprof->setAttribute(Qt::WA_DeleteOnClose);
    connect(newprof, SIGNAL(destroyed(QObject*)), SLOT(on_newProfile_closed()));
    newprof->setModal(true);
    newprof->exec();
}

void MainWindow::on_powerLimSlider_valueChanged(int value)
{
    ui->powerLimSpinBox->setValue(value);
}
void MainWindow::on_powerLimSpinBox_valueChanged(int arg1)
{
    ui->powerLimSlider->setValue(arg1);
}
void MainWindow::on_memClkSlider_valueChanged(int value)
{
    ui->memClkSpinBox->setValue(value);
}
void MainWindow::on_memClkSpinBox_valueChanged(int arg1)
{
    ui->memClkSlider->setValue(arg1);
}
void MainWindow::on_voltageSlider_valueChanged(int value)
{
    ui->voltageSpinBox->setValue(value);
}
void MainWindow::on_voltageSpinBox_valueChanged(int arg1)
{
    ui->voltageSlider->setValue(arg1);
}
void MainWindow::on_fanSlider_valueChanged(int value)
{
    ui->fanSpinBox->setValue(value);
    fanUpdaterDisablerTimer->start(5000);
    fanUpdaterDisablerTimer->setSingleShot(true);
    disconnect(fanUpdateTimer, SIGNAL(timeout()), this, SLOT(fanSpeedUpdater()));
    connect(fanUpdaterDisablerTimer, SIGNAL(timeout()), this, SLOT(enableFanUpdater()));
}
void MainWindow::on_fanSpinBox_valueChanged(int arg1)
{
    ui->fanSlider->setValue(arg1);
}
void MainWindow::enableFanUpdater()
{
    connect(fanUpdateTimer, SIGNAL(timeout()), this, SLOT(fanSpeedUpdater()));
}
void MainWindow::on_applyButton_clicked()
{
    QSettings settings("tuxclocker");
    settings.beginGroup("General");
    QString prevProfile = settings.value("currentProfile").toString();
    settings.setValue("currentProfile", currentProfile);

    applyGPUSettings();
    // Query the maximum offsets
    types->queryGPUCurrentMaxClocks(currentGPUIndex);
    curmaxmemclk->setText(1, QString::number(types->GPUList[currentGPUIndex].maxMemClk) + "MHz");
    curmaxclk->setText(1, QString::number(types->GPUList[currentGPUIndex].maxCoreClk) + "MHz");
}
void MainWindow::on_editFanCurveButton_pressed()
{
    editProfile *editProf = new editProfile(this);
    editProf->setAttribute(Qt::WA_DeleteOnClose);
    connect(editProf, SIGNAL(destroyed(QObject*)), SLOT(on_editProfile_closed()));
    editProf->setModal(true);
    editProf->exec();

}
void MainWindow::on_editProfile_closed()
{
    // Clear the existing curve points and load the new ones
    qDebug() << "dialog closed";
    xCurvePoints.clear();
    yCurvePoints.clear();
    loadProfileSettings();
}

void MainWindow::on_fanModeComboBox_currentIndexChanged(int index)
{
    types->GPUList[currentGPUIndex].fanControlMode = index;
}

void MainWindow::on_actionManage_profiles_triggered()
{
    newProfile *np = new newProfile(this);
    np->setAttribute(Qt::WA_DeleteOnClose);
    connect(np, SIGNAL(destroyed(QObject*)), SLOT(on_newProfile_closed()));
    np->setModal(false);
    np->exec();
}

void MainWindow::on_GPUComboBox_currentIndexChanged(int index)
{
    currentGPUIndex = index;
    // Change latest UUID and load settings for the GPU
    QSettings settings("tuxclocker");
    settings.setValue("General/latestUUID", types->GPUList[index].uuid);
    // Call the NVML setup function so the index of the device struct is updated
    types->setupGPUSecondary(currentGPUIndex);
    types->queryGPUPowerLimitLimits(currentGPUIndex);
    types->queryGPUPowerLimit(currentGPUIndex);
    types->queryGPUPowerLimitAvailability(currentGPUIndex);
    types->queryGPUCurrentMaxClocks(currentGPUIndex);
    // Change the slider ranges and availabilities according to the GPU
    if (types->GPUList[index].overClockAvailable) {
        ui->frequencySlider->setRange(types->GPUList[index].minCoreClkOffset, types->GPUList[index].maxCoreClkOffset);
    } else {
        ui->frequencySlider->setEnabled(false);
        ui->frequencySpinBox->setEnabled(false);
    }

    if (types->GPUList[index].memOverClockAvailable) {
        ui->memClkSlider->setRange(types->GPUList[index].minMemClkOffset, types->GPUList[index].maxMemClkOffset);
    } else {
        ui->memClkSlider->setEnabled(false);
        ui->memClkSpinBox->setEnabled(false);
    }

    if (types->GPUList[index].overVoltAvailable) {
        ui->voltageSlider->setRange(types->GPUList[index].minVoltageOffset, types->GPUList[index].maxVoltageOffset);
    } else {
        ui->voltageSlider->setEnabled(false);
        ui->voltageSpinBox->setEnabled(false);
    }
    if (!types->GPUList[index].manualFanCtrlAvailable) {
        // If manual fan control is not available for the GPU, disable the option
        QStandardItemModel *model = qobject_cast<QStandardItemModel*>(ui->fanModeComboBox->model());
        QModelIndex manualModeIndex = model->index(1, ui->fanModeComboBox->modelColumn());
        QStandardItem *manualMode = model->itemFromIndex(manualModeIndex);
        manualMode->setEnabled(false);
        manualMode->setToolTip("Manual fan control is not available for current GPU");
    }
    loadProfileSettings();
    // Update maximum clocks
    curmaxmemclk->setText(1, QString::number(types->GPUList[index].maxMemClk) + "MHz");
    curmaxclk->setText(1, QString::number(types->GPUList[index].maxCoreClk) + "MHz");
}


void MainWindow::on_amdPstateButton_pressed()
{
    amdPstateEditor *ps = new amdPstateEditor;
    ps->generateUI(types, currentGPUIndex);
    ps->setModal(true);
    ps->exec();
}
