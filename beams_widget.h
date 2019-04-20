#ifndef BEAMS_WIDGET_H
#define BEAMS_WIDGET_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsSimpleTextItem>

namespace Ui {
class BeamsWidget;
}

class BeamsWidget : public QWidget
{
    Q_OBJECT

public:

    // Constructor and destructor
    explicit BeamsWidget(QWidget *parent = 0);
    ~BeamsWidget();

    // Window style
    void setup_style();

    // Connections setup
    void make_connections();

public slots:

    // Change type of beam
    void change_beam();

    // Change height of beam
    void change_beam_height(int beam_height);

    // Populate types of steel
    void populate_steel_types();

    // Relation between linear load and area load
    void switch_to_linear();
    void switch_to_area();
    void calculate_linear_load();

    // Re-drawing the structure case
    void redraw_structure_case(int case_index);

    // Structure calculations
    void structure_calculation();


private:

    // UI
    Ui::BeamsWidget *ui;

    // Graphics scene
    QGraphicsScene *graphics_scene;

};

#endif // BEAMS_WIDGET_H
