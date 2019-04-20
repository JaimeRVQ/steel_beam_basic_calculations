#include "beams_widget.h"
#include "ui_beams_widget.h"
#include "beam_infos.h"
#include <iostream>

BeamsWidget::BeamsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BeamsWidget)
{
    // Setup of UI
    ui->setupUi(this);

    // Graphics scene
    this->graphics_scene = new QGraphicsScene;
    ui->gw->setScene(this->graphics_scene);

    // Calls to member functions to setup the widget
    this->setup_style();
    this->make_connections();
    this->change_beam();
    this->populate_steel_types();
    this->redraw_structure_case(0);
}

BeamsWidget::~BeamsWidget()
{
    delete ui;
}

void BeamsWidget::setup_style()
{
    this->setFixedSize(881, 576);
    this->setWindowTitle("Beams basic structure cases");
    this->setWindowIcon(QIcon(":/imgs/resources/plain_icon.png"));
}

void BeamsWidget::make_connections()
{
    // Functions triggered by change of beam or beam height
    connect(ui->beam_type, SIGNAL(currentIndexChanged(int)), this, SLOT(change_beam()));
    connect(ui->beam_heights, SIGNAL(currentIndexChanged(int)), this, SLOT(change_beam_height(int)));

    // Relation between linear and area load
    connect(ui->linear_check, SIGNAL(clicked(bool)), this, SLOT(switch_to_linear()));
    connect(ui->area_check, SIGNAL(clicked(bool)), this, SLOT(switch_to_area()));
    connect(ui->area_load_value, SIGNAL(valueChanged(double)), this, SLOT(calculate_linear_load()));
    connect(ui->effective_width, SIGNAL(valueChanged(double)), this, SLOT(calculate_linear_load()));

    // Switch between structure cases (needs redrawing of graphic)
    connect(ui->structure_case, SIGNAL(currentIndexChanged(int)), this, SLOT(redraw_structure_case(int)));

    // Changes that trigger re-calculation of structure case
    connect(ui->actions_coeficient, SIGNAL(valueChanged(double)), this, SLOT(structure_calculation()));
    connect(ui->material_coeficient, SIGNAL(valueChanged(double)), this, SLOT(structure_calculation()));
    connect(ui->steel_types, SIGNAL(currentIndexChanged(int)), this, SLOT(structure_calculation()));
    connect(ui->lin_load_value, SIGNAL(valueChanged(double)), this, SLOT(structure_calculation()));
    connect(ui->spacing, SIGNAL(valueChanged(double)), this, SLOT(structure_calculation()));
}

void BeamsWidget::change_beam()
{
    QString beam_type = ui->beam_type->currentText();

    ui->beam_heights->clear();

    if (beam_type == "IPE")
    {
        ui->beam_label->setPixmap(QPixmap(":/imgs/resources/profiles_ipe.png"));
        int number_of_rows = sizeof (IPE_PROPERTIES) / sizeof(IPE_PROPERTIES[0]);
        for (int i=0; i < number_of_rows; i++)
        {
            QString height = QString::number(IPE_PROPERTIES[i][0]);
            ui->beam_heights->addItem(height);
        }
    }
    else if (beam_type == "HEB")
    {
        ui->beam_label->setPixmap(QPixmap(":/imgs/resources/profiles_heb.png"));
        int number_of_rows = sizeof (HEB_PROPERTIES) / sizeof(HEB_PROPERTIES[0]);
        for (int i=0; i < number_of_rows; i++)
        {
            QString height = QString::number(HEB_PROPERTIES[i][0]);
            ui->beam_heights->addItem(height);
        }
    }
    else if (beam_type == "UPN")
    {
        ui->beam_label->setPixmap(QPixmap(":/imgs/resources/profiles_upn.png"));
        int number_of_rows = sizeof (UPN_PROPERTIES) / sizeof(UPN_PROPERTIES[0]);
        for (int i=0; i < number_of_rows; i++)
        {
            QString height = QString::number(UPN_PROPERTIES[i][0]);
            ui->beam_heights->addItem(height);
        }
    }
    else if (beam_type == "# Pipe")
    {
        ui->beam_label->setPixmap(QPixmap(":/imgs/resources/profiles_square_pipe.png"));
        int number_of_rows = sizeof (SQUARE_PIPE_PROPERTIES) / sizeof(SQUARE_PIPE_PROPERTIES[0]);
        for (int i=0; i < number_of_rows; i++)
        {
            QString height = QString::number(SQUARE_PIPE_PROPERTIES[i][0]);
            ui->beam_heights->addItem(height);
        }
    }
    else if (beam_type == "Ø Pipe")
    {
        ui->beam_label->setPixmap(QPixmap(":/imgs/resources/profiles_round_pipe.png"));
        int number_of_rows = sizeof (ROUND_PIPE_PROPERTIES) / sizeof(ROUND_PIPE_PROPERTIES[0]);
        for (int i=0; i < number_of_rows; i++)
        {
            QString height = QString::number(ROUND_PIPE_PROPERTIES[i][0]);
            ui->beam_heights->addItem(height);
        }
    }

    structure_calculation();
}

void BeamsWidget::change_beam_height(int beam_height)
{
    QString beam_type = ui->beam_type->currentText();

    ui->h->setEnabled(true); ui->b->setEnabled(true); ui->A->setEnabled(true);
    ui->e->setEnabled(true); ui->z->setEnabled(true); ui->Ar->setEnabled(true);
    ui->Wx->setEnabled(true); ui->Ix->setEnabled(true); ui->ix->setEnabled(true); ui->Sx->setEnabled(true);
    ui->Wy->setEnabled(true); ui->Iy->setEnabled(true); ui->iy->setEnabled(true); ui->c->setEnabled(true);

    if (beam_type == "IPE")
    {
        ui->h->setValue(IPE_PROPERTIES[beam_height][0]);
        ui->b->setValue(IPE_PROPERTIES[beam_height][1]);
        ui->A->setValue(IPE_PROPERTIES[beam_height][2]);
        ui->e->setValue(IPE_PROPERTIES[beam_height][9]);
        ui->z->setValue(IPE_PROPERTIES[beam_height][10]);
        ui->Ar->setValue(IPE_PROPERTIES[beam_height][11]);

        ui->Wx->setValue(IPE_PROPERTIES[beam_height][3]);
        ui->Ix->setValue(IPE_PROPERTIES[beam_height][4]);
        ui->ix->setValue(IPE_PROPERTIES[beam_height][5]);
        ui->Sx->setValue(IPE_PROPERTIES[beam_height][12]);

        ui->Wy->setValue(IPE_PROPERTIES[beam_height][6]);
        ui->Iy->setValue(IPE_PROPERTIES[beam_height][7]);
        ui->iy->setValue(IPE_PROPERTIES[beam_height][8]);

        // Unused values
        ui->c->setEnabled(false);
        ui->c->setValue(0);
    }
    else if (beam_type == "HEB")
    {
        ui->h->setValue(HEB_PROPERTIES[beam_height][0]);
        ui->b->setValue(HEB_PROPERTIES[beam_height][1]);
        ui->A->setValue(HEB_PROPERTIES[beam_height][2]);
        ui->e->setValue(HEB_PROPERTIES[beam_height][9]);
        ui->z->setValue(HEB_PROPERTIES[beam_height][10]);
        ui->Ar->setValue(HEB_PROPERTIES[beam_height][11]);

        ui->Wx->setValue(HEB_PROPERTIES[beam_height][3]);
        ui->Ix->setValue(HEB_PROPERTIES[beam_height][4]);
        ui->ix->setValue(HEB_PROPERTIES[beam_height][5]);
        ui->Sx->setValue(HEB_PROPERTIES[beam_height][12]);

        ui->Wy->setValue(HEB_PROPERTIES[beam_height][6]);
        ui->Iy->setValue(HEB_PROPERTIES[beam_height][7]);
        ui->iy->setValue(HEB_PROPERTIES[beam_height][8]);

        // Unused values
        ui->c->setEnabled(false);
        ui->c->setValue(0);
    }
    else if (beam_type == "UPN")
    {
        ui->h->setValue(UPN_PROPERTIES[beam_height][0]);
        ui->b->setValue(UPN_PROPERTIES[beam_height][1]);
        ui->A->setValue(UPN_PROPERTIES[beam_height][2]);
        ui->e->setValue(UPN_PROPERTIES[beam_height][9]);

        ui->Wx->setValue(UPN_PROPERTIES[beam_height][3]);
        ui->Ix->setValue(UPN_PROPERTIES[beam_height][4]);
        ui->ix->setValue(UPN_PROPERTIES[beam_height][5]);
        ui->Sx->setValue(UPN_PROPERTIES[beam_height][11]);

        ui->Wy->setValue(UPN_PROPERTIES[beam_height][6]);
        ui->Iy->setValue(UPN_PROPERTIES[beam_height][7]);
        ui->iy->setValue(UPN_PROPERTIES[beam_height][8]);

        ui->c->setValue(UPN_PROPERTIES[beam_height][10]);

        // Unused values
        ui->z->setEnabled(false);
        ui->z->setValue(0);
        ui->Ar->setEnabled(false);
        ui->Ar->setValue(0);
    }
    else if (beam_type == "# Pipe")
    {
        ui->h->setValue(SQUARE_PIPE_PROPERTIES[beam_height][0]);
        ui->e->setValue(SQUARE_PIPE_PROPERTIES[beam_height][1]);
        ui->A->setValue(SQUARE_PIPE_PROPERTIES[beam_height][2]);

        ui->Wx->setValue(SQUARE_PIPE_PROPERTIES[beam_height][3]);
        ui->Ix->setValue(SQUARE_PIPE_PROPERTIES[beam_height][4]);
        ui->ix->setValue(SQUARE_PIPE_PROPERTIES[beam_height][5]);

        ui->Wy->setValue(SQUARE_PIPE_PROPERTIES[beam_height][3]);
        ui->Iy->setValue(SQUARE_PIPE_PROPERTIES[beam_height][4]);
        ui->iy->setValue(SQUARE_PIPE_PROPERTIES[beam_height][5]);

        // Unused values
        ui->z->setEnabled(false);
        ui->z->setValue(0);
        ui->Ar->setEnabled(false);
        ui->Ar->setValue(0);
        ui->Sx->setEnabled(false);
        ui->Sx->setValue(0);
        ui->c->setEnabled(false);
        ui->c->setValue(0);
        ui->b->setEnabled(false);
        ui->b->setValue(0);
    }  
    else if (beam_type == "Ø Pipe")
    {
        ui->h->setValue(ROUND_PIPE_PROPERTIES[beam_height][0]);
        ui->e->setValue(ROUND_PIPE_PROPERTIES[beam_height][1]);
        ui->A->setValue(ROUND_PIPE_PROPERTIES[beam_height][2]);

        ui->Wx->setValue(ROUND_PIPE_PROPERTIES[beam_height][3]);
        ui->Ix->setValue(ROUND_PIPE_PROPERTIES[beam_height][4]);
        ui->ix->setValue(ROUND_PIPE_PROPERTIES[beam_height][5]);

        ui->Wy->setValue(ROUND_PIPE_PROPERTIES[beam_height][3]);
        ui->Iy->setValue(ROUND_PIPE_PROPERTIES[beam_height][4]);
        ui->iy->setValue(ROUND_PIPE_PROPERTIES[beam_height][5]);

        // Unused values
        ui->z->setEnabled(false);
        ui->z->setValue(0);
        ui->Ar->setEnabled(false);
        ui->Ar->setValue(0);
        ui->Sx->setEnabled(false);
        ui->Sx->setValue(0);
        ui->c->setEnabled(false);
        ui->c->setValue(0);
        ui->b->setEnabled(false);
        ui->b->setValue(0);
    }

    structure_calculation();
}

void BeamsWidget::populate_steel_types()
{
    int number_of_steel_types = sizeof(STEEL_ELASTIC_LIMITS)/sizeof(STEEL_ELASTIC_LIMITS[0]);
    for (int i=0; i < (number_of_steel_types); i++)
    { 
        QString steel_type = QString("Steel: S") + QString::number(STEEL_ELASTIC_LIMITS[i]*10);
        ui->steel_types->addItem(steel_type);
    }
}

void BeamsWidget::switch_to_linear()
{
    ui->linear_check->setChecked(true);
    ui->linear_load_frame->setEnabled(true);

    ui->area_check->setChecked(false);
    ui->area_load_frame->setEnabled(false);

    structure_calculation();
}

void BeamsWidget::switch_to_area()
{
    ui->area_check->setChecked(true);
    ui->area_load_frame->setEnabled(true);

    ui->linear_check->setChecked(false);
    ui->linear_load_frame->setEnabled(false);

    calculate_linear_load();
    structure_calculation();
}

void BeamsWidget::calculate_linear_load()
{
    ui->lin_load_value->setValue(ui->area_load_value->value() * ui->effective_width->value());
}

void BeamsWidget::redraw_structure_case(int case_index)
{
    this->graphics_scene->clear();

    // Basic pens and brushes
    QPen black_pen;
    black_pen.setColor(Qt::black);
    black_pen.setWidth(3);

    QBrush white_brush;
    white_brush.setStyle(Qt::SolidPattern);
    white_brush.setColor(Qt::white);

    QPen moment_pen;
    moment_pen.setColor(Qt::blue);
    moment_pen.setWidth(2);
    QBrush moment_brush;
    moment_brush.setStyle(Qt::SolidPattern);
    moment_brush.setColor(QColor(0,0,255,50));

    // Base guidelines (drawing area is (-190, 190) x (-120, 120))
    QLine mid_marker;
    mid_marker.setP1(QPoint(0, -40));
    mid_marker.setP2(QPoint(0, 40));

    QLine horizontal_division;
    horizontal_division.setP1(QPoint(-160, 0));
    horizontal_division.setP2(QPoint(160, 0));

    this->graphics_scene->addLine(mid_marker, QPen(Qt::darkGray, 2, Qt::DotLine));
    this->graphics_scene->addLine(horizontal_division, black_pen);

    // Basic text info
    QGraphicsTextItem *positive_info = this->graphics_scene->addText("(+) Positive moment");
    positive_info->setFont(QFont("arial", 8));
    positive_info->setDefaultTextColor(Qt::darkGreen);
    positive_info->setPos(-50, 100);

    QGraphicsTextItem *negative_info = this->graphics_scene->addText("(-) Negative moment");
    negative_info->setFont(QFont("arial", 8));
    negative_info->setDefaultTextColor(Qt::darkRed);
    negative_info->setPos(-50, -100);

    // Base structure symbols
    QLine left_fixed;
    left_fixed.setP1(QPoint(-160, -30));
    left_fixed.setP2(QPoint(-160, 30));
    QLine right_fixed;
    right_fixed.setP1(QPoint(160, -30));
    right_fixed.setP2(QPoint(160, 30));

    QRect left_roller;
    left_roller.setRect(-165, 0, 10, 10);
    QRect right_roller;
    right_roller.setRect(155, 0, 10, 10);

    QPolygon right_pinned;
    right_pinned.append(QPoint(160,0));
    right_pinned.append(QPoint(165,10));
    right_pinned.append(QPoint(155,10));


    // STRUCTURE CASES //
    if (case_index == 0) // Roller-pinned
    {

        QPainterPath isostatic_path;
        isostatic_path.moveTo(-160, 0);
        isostatic_path.quadTo(QPoint(0, 150), QPoint(160,0));
        this->graphics_scene->addPath(isostatic_path, moment_pen, moment_brush);

        // Structure symbols on top
        this->graphics_scene->addEllipse(left_roller, black_pen, white_brush);
        this->graphics_scene->addPolygon(right_pinned, black_pen, white_brush);
    }

    else if (case_index == 1) // Cantilever
    {
        QPainterPath cantilever_path;
        cantilever_path.moveTo(-160, 0);
        cantilever_path.lineTo(-160, -90);
        cantilever_path.quadTo(QPoint(0, 0), QPoint(160,0));
        this->graphics_scene->addPath(cantilever_path, moment_pen, moment_brush);

        // Structure symbols on top
        this->graphics_scene->addLine(left_fixed, black_pen);
    }

    else if (case_index == 2) // Fixed-roller
    {
        QPainterPath fixed_roller_path;
        fixed_roller_path.moveTo(-160, 0);
        fixed_roller_path.lineTo(-160, -70);
        fixed_roller_path.quadTo(QPoint(0, 150), QPoint(160,0));
        this->graphics_scene->addPath(fixed_roller_path, moment_pen, moment_brush);

        // Structure symbols on top
        this->graphics_scene->addLine(left_fixed, black_pen);
        this->graphics_scene->addEllipse(right_roller, black_pen, white_brush);

        // Special info
        QGraphicsTextItem *special_info = this->graphics_scene->addText("*Note mid/max moment\n is slightly off center");
        special_info->setFont(QFont("arial", 7));
        special_info->setDefaultTextColor(Qt::black);
        special_info->setPos(50, 60);
    }

    else if (case_index == 3) // Fixed-fixed
    {
        QPainterPath bi_fixed_path;
        bi_fixed_path.moveTo(-160, 0);
        bi_fixed_path.lineTo(-160, -70);
        bi_fixed_path.quadTo(QPoint(0, 200), QPoint(160,-70));
        bi_fixed_path.lineTo(160,0);
        this->graphics_scene->addPath(bi_fixed_path, moment_pen, moment_brush);

        // Structure symbols on top
        this->graphics_scene->addLine(left_fixed, black_pen);
        this->graphics_scene->addLine(right_fixed, black_pen);
    }

    structure_calculation();
}

void BeamsWidget::structure_calculation()
{
    //-------------------//
    // PROPERTIES NEEDED //
    //-------------------//

    // BEAM RELATED PROPERTIES
    double W = ui->Wx->value();

    // STEEL PROPERTIES
    double characteristic_resistence = STEEL_ELASTIC_LIMITS[ui->steel_types->currentIndex()];
    double safety_coeficient = ui->actions_coeficient->value() * ui->material_coeficient->value();
    double max_tension_with_safety = floor(characteristic_resistence / safety_coeficient);

    // LOAD RELATED PROPERTIES
    double q = ui->lin_load_value->value();
    double L = ui->spacing->value();

    // MOMENTS AND SHEARS
    double left_moment, center_moment, right_moment;
    double left_shear, center_shear, right_shear;
    double left_tension, center_tension, right_tension;

    //-----------------//
    // STRUCTURE CASES //
    //-----------------//
    int current_structure_case = ui->structure_case->currentIndex();
    switch (current_structure_case)
    {
        case 0: // Roller-pinned
            left_moment = 0;        center_moment = q*L*L/8;  right_moment = 0;
            left_shear = -1*q*L/2;  center_shear = 0;         right_shear = q*L/2;
            break;

        case 1: // Cantilever
            left_moment = -1*q*L*L/2;  center_moment = 0;  right_moment = 0;
            left_shear = -1*q*L;       center_shear = 0;   right_shear = 0;
            break;

        case 2: // Fixed-roller
            left_moment = -1*q*L*L/8;  center_moment = 9*q*L*L/128;  right_moment = 0;
            left_shear = -5*q*L/8;     center_shear = 0;             right_shear = 3*q*L/8;
            break;

        case 3: // Fixed-fixed
            left_moment = -1*q*L*L/12;  center_moment = q*L*L/24;  right_moment = -1*q*L*L/12;
            left_shear = -1*q*L/2;      center_shear = 0;          right_shear = q*L/2;
            break;
    }

    //---------//
    // RESULTS //
    //---------//

    // TENSION
    (left_moment != 0)?(left_tension = left_moment * 100 / W):(left_tension = 0);
    (center_moment != 0)?(center_tension = center_moment * 100 / W):(center_tension = 0);
    (right_moment != 0)?(right_tension = right_moment * 100 / W):(right_tension = 0);

    // Establishing the values
    ui->left_moment_BOX->setValue(left_moment);
    ui->mid_moment_BOX->setValue(center_moment);
    ui->right_moment_BOX->setValue(right_moment);

    ui->left_shear_BOX->setValue(left_shear);
    ui->mid_shear_BOX->setValue(center_shear);
    ui->right_shear_BOX->setValue(right_shear);

    ui->left_tension_BOX->setValue(left_tension);
    ui->mid_tension_BOX->setValue(center_tension);
    ui->right_tension_BOX->setValue(right_tension);

    // Tension checks
    (std::abs(left_tension)>max_tension_with_safety)?(ui->left_tension_BOX->setStyleSheet("color:red")):(ui->left_tension_BOX->setStyleSheet(""));
    (std::abs(center_tension)>max_tension_with_safety)?(ui->mid_tension_BOX->setStyleSheet("color:red")):(ui->mid_tension_BOX->setStyleSheet(""));
    (std::abs(right_tension)>max_tension_with_safety)?(ui->right_tension_BOX->setStyleSheet("color:red")):(ui->right_tension_BOX->setStyleSheet(""));

}
