#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>

/*
*   Constructor of MainWindow
*
*/
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    matrix = new Matrix(15,15);
    this->p = new Points();
    this->scene = new QGraphicsScene(this);
    this->scene->setSceneRect(-15,-10,700,700);
    ui->setupUi(this);
    ui->graphicsView->setScene(this->scene);
    this->ui->costo_label->hide();
    this->ui->costo_labelt->hide();
    ui->comboBox->addItem("Monkey");
    ui->comboBox->addItem("Human");
    ui->comboBox->addItem("Octopus");
    QObject::connect(matrix ,SIGNAL(ChangeActiveBox(Coordinates)),
                     this,SLOT(updateActiveBox(Coordinates)));
    //QObject::connect(this->alg,SIGNAL(move()),this,SLOT(on_move())); //updateMaze()

}
/*
*   Destructor of MainWindow
*
*/
MainWindow::~MainWindow()
{
    delete matrix;
    delete ui;
}
/****************************************************************************************************/
/****************************************************************************************************/

/*
*   Function to load a map from file using a QFileDialog
*
*   If the source file isn't the same size of witch expect this function the program crashes
*/
void MainWindow::loadMap(){
    QString filename = QFileDialog::getOpenFileName(this,tr("open file"),"C://Users//Alejandro//Dropbox//Qt//IA","all file (*.*)");
    unsigned int m = 0;
    Casilla *aux;
    /*
     * Leer el archivo
    */
    QFile source(filename);
    if(source.open(QIODevice::ReadOnly)){
        QTextStream in(&source);

        while(!in.atEnd()) {
            bool con;
            QString line = in.readLine();
            QStringList fields = line.split(",");
            for (int i = 0; i < fields.size(); ++i){
                //crea las casillas las guarda en la matriz
                aux = new Casilla(0,0,35,35 );
                aux->setX((37*i));
                aux->setY((37*m));
                aux->setType(fields.at(i).toInt(&con,10));
                aux->View(false);
                aux->FinalPoint(false);
                aux->InicialPoint(false);
                aux->Path(false);

                //this->matrix->getValueAt(i,m)->setX((37*i));
                //this->matrix->getValueAt(i,m)->setY((37*m));
                //this->matrix->getValueAt(i,m)->setType(fields.at(i).toInt(&con,10));
                //this->matrix->getValueAt(i,m)->View(false);
                //this->matrix->getValueAt(i,m)->FinalPoint(false);
                //this->matrix->getValueAt(i,m)->InicialPoint(false);
                //this->matrix->getValueAt(i,m)->Path(false);
                QObject::connect(aux,SIGNAL(focusSignal(Coordinates)),this->matrix,SLOT(setPoint(Coordinates)));
                QObject::connect(aux,SIGNAL(move(Coordinates,unsigned short int)),this,SLOT(move(Coordinates,unsigned short int)));
                this->matrix->setValueAt(i,m,aux );
            }
            m++;
        }
        source.close();
        this->matrix->Load(true);
        this->ui->actionSet->setEnabled(true);
        ui->actionFinal->setEnabled(true);
        ui->actionInicial->setEnabled(true);
        ui->actionHuman->setEnabled(true);
        ui->actionDark_templ->setEnabled(true);
        ui->actionKey->setEnabled(true);
        ui->actionPortal->setEnabled(true);
        ui->actionOctopius->setEnabled(true);
        ui->actionClear->setEnabled(true);
        ui->actionCover_map->setEnabled(true);
        ui->actionSolve->setEnabled(true);
        ui->actionSolve_2->setEnabled(true);
        ui->actionSolve_3->setEnabled(true);
        ui->actionSolve_4->setEnabled(true);
        ui->actionSolve_5->setEnabled(true);
        ui->actionCosto->setEnabled(true);

    }else{
        QMessageBox::information(0,"error",source.errorString());
        this->matrix->Load(false);
    }

}
/*
*   Function to draw a map
*
*   only change the QBrush of the QGraphicsRectItem
*/
void MainWindow::drawMap(int x, int y,bool remove)
{
    Casilla *aux ;
    int type_t = 0;
    for(int i=0;i<x;i++){
        for(int j=0;j<y;j++){
            aux = this->matrix->getValueAt(i,j);
            if(remove)
                scene->removeItem(aux);


            if( aux->isView() == true){
                aux->setBrush(Qt::darkBlue);
            }else{
                type_t = this->matrix->getValueAt(i,j)->getType() ;
                if(type_t == 0){
                    aux->setBrush(Qt::gray);
                }else if(type_t == 1){
                    aux->setBrush(Qt::white);
                }else if(type_t == 2){
                    aux->setBrush(Qt::blue);
                }else if(type_t == 3){
                    aux->setBrush(Qt::yellow);
                }else if(type_t == 4){
                    aux->setBrush(Qt::green);
                }

            }
            if(aux->isPath()){
                aux->setBrush(Qt::darkYellow);
            }
            if( aux->isInicialPoint()){
                aux->setBrush(Qt::red);
            }else if( aux->isFinalPoint() ){
                aux->setBrush(Qt::magenta);
            }

            scene->addItem(aux);
        }
    }
}

/****************************************************************************************************/
/****************************************************************************************************/
/*
*   Function handle when the action Open has triggered
*/
void MainWindow::on_actionOpen_triggered()
{
    this->loadMap();

    this->drawMap(this->matrix->getm(),this->matrix->getn(),false);
}
/*
*   Function handle when the action Open has triggered
*/
void MainWindow::on_actionNew_triggered()
{
    //this->scene->clear();
    qDebug() << "matrix values" ;

    if( this->matrix->isLoaded())
        this->matrix->print();
}
/*
*   Function handle
*   to set a terrain type
*/
void MainWindow::on_actionSet_triggered()
{
    bool ok;

    QString text = QInputDialog::getText(0, "Set terrain type",
                                         "type :", QLineEdit::Normal,
                                         "", &ok);
    if (ok && !text.isEmpty()) {
        QRegExp re("\\d*");  // a digit (\d), zero or more times (*)
        if (re.exactMatch(text)){
            qDebug() << "inpunt value is all digits" << endl;
            this->matrix->getValueAt(this->activeCor.getX(),this->activeCor.getY())->setType(text.toInt(&ok,10));
            if(this->matrix->getValueAt(this->activeCor.getX(),this->activeCor.getY())->getType() == 0)
                this->matrix->getValueAt(this->activeCor.getX(),this->activeCor.getY())->setBrush(Qt::gray);
            else
                this->matrix->getValueAt(this->activeCor.getX(),this->activeCor.getY())->setBrush(Qt::white);
            this->scene->update();
        }else{
           qDebug() << "inpunt value is invalid" << endl;
        }

        //QMessageBox::information(this,tr("Set type"), QString::number(this->matrix->getValueAt(a,b)->getType() ));
    }

}

/****************************************************************************************************/
/****************************************************************************************************/

/*
*   Function handle when the action Open has triggered
*/
void MainWindow::on_actionInicial_triggered()
{

    this->matrix->setInicialPoint(this->activeCor);
    this->matrix->getValueAt(this->activeCor.getX(),this->activeCor.getY())->setBrush(Qt::red);
    this->matrix->getValueAt(this->activeCor.getX(),this->activeCor.getY())->InicialPoint(true);
    this->matrix->getValueAt(this->activeCor.getX(),this->activeCor.getY())->View(true);
    this->ui->IP->setText(this->activeCor.text());
    this->scene->update();
}
/*
*   Function handle when the action Open has triggered
*/
void MainWindow::on_actionFinal_triggered()
{
    this->matrix->setFinalPoint(this->activeCor);
    this->matrix->getValueAt(this->activeCor.getX(),this->activeCor.getY())->setBrush(Qt::magenta);
    this->matrix->getValueAt(this->activeCor.getX(),this->activeCor.getY())->FinalPoint(true);
    this->ui->FP->setText(this->activeCor.text());
    this->scene->update();
}
/********************************************************************************/
/*
 * slots to assign the points human, octopus
*/
void MainWindow::on_actionKey_triggered()
{
    qDebug() << "Key point"<< endl;
    //this->matrix->setKeyPoint();
    this->p->setkey(this->activeCor);
    this->matrix->getValueAt(this->activeCor)->setBrush(Qt::magenta);
    this->scene->update();
}

void MainWindow::on_actionDark_templ_triggered()
{
    qDebug() << "Dark temple" << endl;
    //this->matrix->setDarkPoint();
    this->p->setDark(this->activeCor);
    this->matrix->getValueAt(this->activeCor)->setBrush(Qt::darkRed);
    this->scene->update();
}

void MainWindow::on_actionHuman_triggered()
{
    qDebug() << "Human" << endl;
    //this->matrix->setDarkPoint();
    this->p->setHuman(this->activeCor);
    this->matrix->getValueAt(this->activeCor)->setBrush(Qt::darkBlue);
    this->scene->update();
}

void MainWindow::on_actionOctopius_triggered()
{
    qDebug() << "Octopus" << endl;
    //this->matrix->setDarkPoint();
    this->p->setOctopus(this->activeCor);
    this->matrix->getValueAt(this->activeCor)->setBrush(Qt::darkGreen);
    this->scene->update();
}

void MainWindow::on_actionPortal_triggered()
{
    qDebug() << "Portal" << endl;
    //this->matrix->setDarkPoint();
    this->p->setPortal(this->activeCor);
    this->matrix->getValueAt(this->activeCor)->setBrush(Qt::darkCyan);
    this->scene->update();
}
/********************************************************************************/
/*
*   Function handle when the action Open has triggered
*/
void MainWindow::onClickChangeType()
{

}

void MainWindow::updateActiveBox(Coordinates c)
{
    this->activeCor = c;
    qDebug() << "updateActiveBox slot was trigger" << endl;
}
/****************************************************************************************************/
/****************************************************************************************************/
/*
*   Functions call to solve de maze
*
*
*/


void MainWindow::on_actionBredth_First_Search_triggered()
{

}

void MainWindow::on_actionDepth_First_Search_triggered()
{

}

void MainWindow::on_actionIterative_Depth_First_Search_triggered()
{

}

void MainWindow::on_actionA_triggered()
{

}

/****************************************************************************************************/
/****************************************************************************************************/
void MainWindow::on_actionCover_map_triggered()
{

    Casilla *aux ;

    for(unsigned int i=0;i<this->matrix->getm();i++){
        for(unsigned int j=0;j<this->matrix->getn();j++){
            aux = this->matrix->getValueAt(i,j);

            if( aux->isInicialPoint()){
                aux->setBrush(Qt::red);
            }else if( aux->isFinalPoint() ){
                aux->setBrush(Qt::magenta);
            }else{
                aux->setBrush(Qt::black);
            }
        }
    }
    this->scene->update();
    aux = this->matrix->getValueAt(this->matrix->getInicialPoint());
    aux->setFocus();
}

void MainWindow::move(Coordinates c,unsigned short int a)
{
    qDebug() << "make a move to solve the maze  :" << c.getX() << " :" << c.getY()<< endl;
    emit make_move(c,a);
}


void MainWindow::updateView()
{
    qDebug() << "update view " << endl;
    this->drawMap(this->matrix->getm(),this->matrix->getn(),true);
    this->scene->update();

}

void MainWindow::updateOne()
{
    this->scene->update();
}

void MainWindow::on_actionClear_triggered()
{
    for (unsigned int i = 0; i < this->matrix->getn(); i++){
        //crea las casillas las guarda en la matriz
        for (unsigned int m = 0; m < this->matrix->getm(); m++){
            this->matrix->getValueAt(i,m)->View(false);
            this->matrix->getValueAt(i,m)->FinalPoint(false);
            this->matrix->getValueAt(i,m)->InicialPoint(false);
            this->matrix->getValueAt(i,m)->Path(false);
        }
    }
    this->drawMap(this->matrix->getm(),this->matrix->getn(),true);
    this->ui->costo_label->setText("");
    this->ui->costo_labelt->hide();
    this->ui->costo_labelt->hide();
    this->ui->FP->setText("");
    this->ui->IP->setText("");
}

void MainWindow::on_actionSolve_triggered()
{
    BFS *bfs = new BFS();
    bfs->setMatrix(matrix);
    QObject::connect(bfs,SIGNAL(updateMaze()),this,SLOT(updateView()));
    bfs->solve();
    ui->actionOriginal->setEnabled(true);
    ui->actionSolution->setEnabled(true);
}

void MainWindow::on_actionSolve_2_triggered()
{
    DFS *dfs = new DFS();
    dfs->setMatrix(matrix);
    QObject::connect(dfs,SIGNAL(updateMaze()),this,SLOT(updateView()));
    dfs->solve();
    ui->actionOriginal->setEnabled(true);
    ui->actionSolution->setEnabled(true);
}

void MainWindow::on_actionSolve_3_triggered()
{
    bool ok;
    int i,p;
    QString text = QInputDialog::getText(0, "Set I",
                                         "Value of I:", QLineEdit::Normal,
                                         "", &ok);
    if (ok && !text.isEmpty()) {
        QRegExp re("\\d*");  // a digit (\d), zero or more times (*)
        if (re.exactMatch(text)){

            i = text.toInt(&ok,10);
            qDebug() << "inpunt value is all digits  " << i  <<endl;
        }else{
           qDebug() << "inpunt value is invalid" << endl;
        }
    }
    text = QInputDialog::getText(0, "Set I",
                                         "Value of I:", QLineEdit::Normal,
                                         "", &ok);
    if (ok && !text.isEmpty()) {
        QRegExp re("\\d*");  // a digit (\d), zero or more times (*)
        if (re.exactMatch(text)){

            p = text.toInt(&ok,10);
            qDebug() << "inpunt value is all digits  " << p  <<endl;
        }else{
           qDebug() << "inpunt value is invalid" << endl;
        }
    }

    IDF *idf = new IDF();
    idf->setMatrix(matrix);
    QObject::connect(idf,SIGNAL(updateMaze()),this,SLOT(updateView()));
    idf->setIP((unsigned)i,(unsigned)p);
    idf->solve();
    ui->actionOriginal->setEnabled(true);
    ui->actionSolution->setEnabled(true);
}

void MainWindow::on_actionSolve_4_triggered()
{
    qDebug() << " solve using a *" <<endl;
    Entity *entity;
    if(ui->comboBox->currentIndex() == 0){
        entity = new Monkey();
    }else if(ui->comboBox->currentIndex() == 1){
        entity = new Human();
    }else if(ui->comboBox->currentIndex() == 2){
        entity = new Octopus();
    }


    this->ui->LEMountain->setText(QString::number( entity->getCost(0) ) );
    this->ui->LEMountain->setEnabled(false);
    this->ui->LEEarth->setText(QString::number( entity->getCost(1) ));
    this->ui->LEEarth->setEnabled(false);
    this->ui->LEWater->setText(QString::number( entity->getCost(2) ));
    this->ui->LEWater->setEnabled(false);
    this->ui->LEForest->setText(QString::number( entity->getCost(4) ));
    this->ui->LEForest->setEnabled(false);
    this->ui->LESand->setText(QString::number( entity->getCost(3) ));
    this->ui->LESand->setEnabled(false);

    AStar *astar = new AStar();
    astar->setMatrix(matrix);
    astar->setEntity(entity);
    QObject::connect(astar,SIGNAL(updateMaze()),this,SLOT(updateView()));
    astar->solve();
    ui->actionOriginal->setEnabled(true);
    ui->actionSolution->setEnabled(true);
}

void MainWindow::on_actionCosto_triggered()
{
    Route *r = new Route();
    r->setMatrix(matrix);
    r->setPoints(this->p);
    qDebug()<<"Key "<< p->getKey().getX() << " : " << p->getKey().getY() << endl;
    qDebug()<<"Dark "<< p->getDark().getX() << " : " << p->getDark().getY() << endl;
    qDebug()<<"Human "<< p->getHuman().getX() << " : " << p->getHuman().getY() << endl;
    qDebug()<<"Octo "<< p->getOctopus().getX() << " : " << p->getOctopus().getY() << endl;
    qDebug()<<"Portal"<< p->getPortal().getX() << " : " << p->getPortal().getY() << endl;
    r->calc();
    this->ui->costo_label->setText(QString::number(r->getCosto()));
    this->ui->costo_label->show();
    this->ui->costo_labelt->show();
    this->ui->lineEditHuman->setText(r->getHuman());
    this->ui->lineEditOctopus->setText(r->getOctopus());
    ui->actionOriginal->setEnabled(true);
    ui->actionSolution->setEnabled(true);
}

void MainWindow::on_actionSolve_5_triggered()
{
    s = new Solver();
    s->setMatrix(this->matrix);
    QObject::connect(s,SIGNAL(updateMaze()),this,SLOT(updateOne()));
    QObject::connect(this,SIGNAL(make_move(Coordinates,unsigned short int)),s,SLOT(on_move(Coordinates,unsigned short int)));
    s->solveManual();

}

void MainWindow::on_actionOriginal_triggered()
{
    Casilla *aux ;
    int type_t = 0;
    for(int i=0;i<15;i++){
        for(int j=0;j<15;j++){
            aux = this->matrix->getValueAt(i,j);
                scene->removeItem(aux);
                type_t = this->matrix->getValueAt(i,j)->getType() ;
                if( aux->isInicialPoint()){
                    aux->setBrush(Qt::red);
                }else if( aux->isFinalPoint() ){
                    aux->setBrush(Qt::magenta);
                }else{
                    if(type_t == 0){
                        aux->setBrush(Qt::gray);
                    }else if(type_t == 1){
                        aux->setBrush(Qt::white);
                    }else if(type_t == 2){
                        aux->setBrush(Qt::blue);
                    }else if(type_t == 3){
                        aux->setBrush(Qt::yellow);
                    }else if(type_t == 4){
                        aux->setBrush(Qt::green);
                    }
                }

            scene->addItem(aux);
        }
    }
}

void MainWindow::on_actionSolution_triggered()
{
    this->drawMap(15,15,true);
}

