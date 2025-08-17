#include "mainwindow.hpp"
#include "../../build/lab3_qt_autogen/include/ui_mainwindow.h"
#include <regex>
#include <sstream>

template <>
int MainWindow::getFromInput<int>( const QString& input ) const {
    std::regex r(R"([+-]?[0-9]+)");
    std::string str = input.toStdString();
    if (!std::regex_match(str, r)) {
        throw Exception(Exception::ErrorCode::INVALID_INPUT);
    }
    return input.toInt();
}

template<>
double MainWindow::getFromInput<double>( const QString& input ) const {
    std::regex r(R"([+-]?[0-9]*\.?[0-9]+([eE][+-]?[0-9]+)?)");
    std::string str = input.toStdString();
    if (!std::regex_match(str, r)) {
        throw Exception(Exception::ErrorCode::INVALID_INPUT);
    }
    return input.toDouble();
}

template<>
std::string MainWindow::getFromInput<std::string>( const QString& input ) const {
    std::string str = input.toStdString();
    if (str.length() > 20) {
        throw Exception(Exception::ErrorCode::INVALID_INPUT);
    }
    return str;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->seqList->setSelectionMode(QAbstractItemView::ExtendedSelection);
}

MainWindow::~MainWindow()
{
    for (int i = 0; i < ui->seqList->count(); ++i) {
        auto* item = ui->seqList->item(i);
        auto tuple = item->data(Qt::UserRole).value<SequenceTuple>();
        delete tuple;
    }
    delete ui;
}

void MainWindow::showError( const std::exception &ex ) {
    QString message = QString( ex.what() );
    statusBar()->showMessage( message, 5000 );
}

void MainWindow::on_createBtn_clicked()
{
    try {
        ICollectionTuple<Sequence, int, double, Person, std::string>* tuple;

        if ( ui->containerType->currentIndex() == SequenceType::FLIST ) {

            if        ( ui->varType->currentIndex() == VarType::INT    ) {
                tuple = WrapFactory::enwrap<int,         FlistSequence>();
            } else if ( ui->varType->currentIndex() == VarType::DOUBLE ) {
                tuple = WrapFactory::enwrap<double,      FlistSequence>();
            } else if ( ui->varType->currentIndex() == VarType::PERSON ) {
                tuple = WrapFactory::enwrap<Person,      FlistSequence>();
            } else if ( ui->varType->currentIndex() == VarType::STRING ) {
                tuple = WrapFactory::enwrap<std::string, FlistSequence>();
            } else {
                throw Exception( Exception::ErrorCode::INVALID_TYPE );
            }

        } else if ( ui->containerType->currentIndex() == SequenceType::STACK ) {

            if        ( ui->varType->currentIndex() == VarType::INT    ) {
                tuple = WrapFactory::enwrap<int,         StackSequence>();
            } else if ( ui->varType->currentIndex() == VarType::DOUBLE ) {
                tuple = WrapFactory::enwrap<double,      StackSequence>();
            } else if ( ui->varType->currentIndex() == VarType::PERSON ) {
                tuple = WrapFactory::enwrap<Person,      StackSequence>();
            } else if ( ui->varType->currentIndex() == VarType::STRING ) {
                tuple = WrapFactory::enwrap<std::string, StackSequence>();
            } else {
                throw Exception( Exception::ErrorCode::INVALID_TYPE );
            }

        } else {
            throw Exception( Exception::ErrorCode::INVALID_TYPE);
        }
        QListWidgetItem* item = new QListWidgetItem( QString::fromStdString( tuple->getCollection()->print() ));
        item->setData(Qt::UserRole, QVariant::fromValue(tuple));
        ui->seqList->addItem(item);
    } catch ( std::exception &ex ) {
        showError(Exception(ex));
    }
}


void MainWindow::on_deleteBtn_clicked()
{
    try {
        QListWidgetItem* item = ui->seqList->takeItem(ui->seqList->currentRow());
        if (item) {
            auto tuple = item->data(Qt::UserRole).value<SequenceTuple>();
            auto clct = tuple->getCollection();
            delete tuple;
            delete item;
        } else {
            throw Exception( Exception::ErrorCode::INVALID_SELECTION ) ;
        }
    } catch ( Exception &ex ) {
        showError(Exception(ex));
    }
}

void MainWindow::on_appendBtn_clicked()
{
    try {    
        QListWidgetItem* item = ui->seqList->item(ui->seqList->currentRow());
        if (item) {
            auto tuple = item->data(Qt::UserRole).value<SequenceTuple>();
            auto clct = tuple->getCollection();

            if ( ui->valueInput->text().isEmpty() ) {
                throw Exception( Exception::ErrorCode::INVALID_INPUT );
            }

            if (!ui->isImmutable->isChecked()) {
                if ( clct->typeId == VarType::INT ) {                 
                    
                    tuple->get<int>()->append( getFromInput<int>( ui->valueInput->text() ) );
                    item->setText( QString::fromStdString( tuple->getCollection()->print()) );

                } else if ( clct->typeId == VarType::DOUBLE ) { 

                    tuple->get<double>()->append( ui->valueInput->text().toDouble() );
                    item->setText( QString::fromStdString( tuple->getCollection()->print()) );

                } else if ( clct->typeId == VarType::STRING ) {

                    tuple->get<std::string>()->append( getFromInput<std::string>( ui->valueInput->text() ) );
                    item->setText( QString::fromStdString( tuple->getCollection()->print()) );

                } else if ( clct->typeId == VarType::PERSON ) {

                    tuple->get<Person>()->append( getFromInput<Person>( ui->valueInput->text() ) );
                    item->setText( QString::fromStdString( tuple->getCollection()->print()) );

                } else {
                    throw Exception( Exception::ErrorCode::INVALID_TYPE ); 
                }
            } else {
                ICollectionTuple<Sequence, int, double, Person, std::string>* changedTuple;
                if ( clct->typeId == VarType::INT ) {

                    auto seq = tuple->get<int>()->appendImmutable( ui->valueInput->text().toInt() );
                    changedTuple = WrapFactory::enwrap<int>(seq);
                    addToList(*changedTuple);

                } else if ( clct->typeId == VarType::DOUBLE ) {

                    auto seq = tuple->get<double>()->appendImmutable( ui->valueInput->text().toDouble() );
                    changedTuple = WrapFactory::enwrap<double>(seq);
                    addToList(*changedTuple);

                } else {
                    throw Exception( Exception::ErrorCode::INVALID_TYPE ); 
                }
            }
        } else {
            throw Exception( Exception::ErrorCode::INVALID_SELECTION ) ;
        }
    } catch ( Exception &ex ) {
        showError(Exception(ex));
    }
}

void MainWindow::on_prependBtn_clicked()
{
    try {
        QListWidgetItem* item = ui->seqList->item(ui->seqList->currentRow());
        if (item) {
            auto tuple = item->data(Qt::UserRole).value<SequenceTuple>();
            auto clct = tuple->getCollection();
            
            if ( ui->valueInput->text().isEmpty() ) {
                throw Exception( Exception::ErrorCode::INVALID_INPUT );
            }
            if (!ui->isImmutable->isChecked()) {
                if ( clct->typeId == VarType::INT ) { 

                    tuple->get<int>()->prepend( getFromInput<int>( ui->valueInput->text() ) );
                    item->setText( QString::fromStdString( tuple->getCollection()->print()) );

                } else if ( clct->typeId == VarType::DOUBLE ) { 

                    tuple->get<double>()->prepend( getFromInput<double>( ui->valueInput->text() ) );
                    item->setText( QString::fromStdString( tuple->getCollection()->print()) );

                } else if ( clct->typeId == VarType::PERSON ) {

                    tuple->get<Person>()->prepend( getFromInput<Person>( ui->valueInput->text() ) );
                    item->setText( QString::fromStdString( tuple->getCollection()->print()) );

                } else if ( clct->typeId == VarType::STRING ) {

                    tuple->get<std::string>()->prepend( getFromInput<std::string>( ui->valueInput->text() ) );
                    item->setText( QString::fromStdString( tuple->getCollection()->print()) );

                } else { throw Exception( Exception::ErrorCode::INVALID_TYPE ); }
            } else {
                ICollectionTuple<Sequence, int, double, Person, std::string>* changedTuple;
                if ( clct->typeId == VarType::INT ) {

                    auto seq = tuple->get<int>()->prependImmutable( getFromInput<int>( ui->valueInput->text() ) );
                    changedTuple = WrapFactory::enwrap<int>(seq);
                    addToList(*changedTuple);

                } else if ( clct->typeId == VarType::DOUBLE ) {

                    auto seq = tuple->get<double>()->prependImmutable( getFromInput<double>( ui->valueInput->text() ) );
                    changedTuple = WrapFactory::enwrap<double>(seq);
                    addToList(*changedTuple);

                } else if ( clct->typeId == VarType::PERSON ) {

                    auto seq = tuple->get<Person>()->prependImmutable( getFromInput<Person>( ui->valueInput->text() ) );
                    changedTuple = WrapFactory::enwrap<Person>(seq);
                    addToList(*changedTuple);

                } else if ( clct->typeId == VarType::STRING ) {

                    auto seq = tuple->get<std::string>()->prependImmutable( getFromInput<std::string>( ui->valueInput->text() ) );
                    changedTuple = WrapFactory::enwrap<std::string>(seq);
                    addToList(*changedTuple);

                } else {
                    throw Exception( Exception::ErrorCode::INVALID_TYPE ); 
                }
            }
        } else {
            throw Exception( Exception::ErrorCode::INVALID_SELECTION ) ;
        }
    } catch ( Exception &ex ) {
        showError(Exception(ex));
    }
}

void MainWindow::on_removeAtBtn_clicked()
{
    try {
        QListWidgetItem* item = ui->seqList->item(ui->seqList->currentRow());
        if (item) {
            auto tuple = item->data(Qt::UserRole).value<SequenceTuple>();
            auto clct = tuple->getCollection();

            if ( ui->index1Input->text().isEmpty() ) {
                throw Exception( Exception::ErrorCode::INVALID_INPUT );
            }
            if (!ui->isImmutable->isChecked()) {
                if ( clct->typeId == VarType::INT ) { 

                    tuple->get<int>()->removeAt( getFromInput<int>( ui->index1Input->text() ) );
                    item->setText( QString::fromStdString( tuple->getCollection()->print()) );

                } else if ( clct->typeId == VarType::DOUBLE ) { 

                    tuple->get<double>()->removeAt( getFromInput<int>( ui->index1Input->text() ) );
                    item->setText( QString::fromStdString( tuple->getCollection()->print()) );

                } else if ( clct->typeId == VarType::PERSON ) {

                    tuple->get<Person>()->removeAt( getFromInput<int>( ui->index1Input->text() ) );
                    item->setText( QString::fromStdString( tuple->getCollection()->print()) );

                } else if ( clct->typeId == VarType::STRING ) {

                    tuple->get<std::string>()->removeAt( getFromInput<int>( ui->index1Input->text() ) );
                    item->setText( QString::fromStdString( tuple->getCollection()->print()) );

                } else { throw Exception( Exception::ErrorCode::INVALID_TYPE ); }
            } else {
                ICollectionTuple<Sequence, int, double, Person, std::string>* changedTuple;
                if ( clct->typeId == VarType::INT ) {

                    auto seq = tuple->get<int>()->removeAtImmutable( getFromInput<int>( ui->index1Input->text() ) );
                    changedTuple = WrapFactory::enwrap<int>(seq);
                    addToList(*changedTuple);

                } else if ( clct->typeId == VarType::DOUBLE ) {

                    auto seq = tuple->get<double>()->removeAtImmutable( getFromInput<int>( ui->index1Input->text() ) );
                    changedTuple = WrapFactory::enwrap<double>(seq);
                    addToList(*changedTuple);

                } else if ( clct->typeId == VarType::PERSON ) {

                    auto seq = tuple->get<Person>()->removeAtImmutable( getFromInput<int>( ui->index1Input->text() ) );
                    changedTuple = WrapFactory::enwrap<Person>(seq);
                    addToList(*changedTuple);

                } else if ( clct->typeId == VarType::STRING ) {

                    auto seq = tuple->get<std::string>()->removeAtImmutable( getFromInput<int>( ui->index1Input->text() ) );
                    changedTuple = WrapFactory::enwrap<std::string>(seq);
                    addToList(*changedTuple);

                } else {
                    throw Exception( Exception::ErrorCode::INVALID_TYPE ); 
                }
            }
        } else {
            throw Exception( Exception::ErrorCode::INVALID_SELECTION ) ;
        }
    } catch ( Exception &ex ) {
        showError(Exception(ex));
    }
}

void MainWindow::on_setAtBtn_clicked()
{
    try {
        QListWidgetItem* item = ui->seqList->item(ui->seqList->currentRow());
        if (item) {
            auto tuple = item->data(Qt::UserRole).value<SequenceTuple>();
            auto clct = tuple->getCollection();

            if ( ui->index1Input->text().isEmpty() || ui->valueInput->text().isEmpty() ) {
                throw Exception( Exception::ErrorCode::INVALID_INPUT );
            }

            if (!ui->isImmutable->isChecked()) {
                if ( clct->typeId == VarType::INT ) { 

                    tuple->get<int>()->setAt( getFromInput<int>( ui->valueInput->text() ),
                                              getFromInput<int>( ui->index1Input->text() ) );
                    item->setText( QString::fromStdString( tuple->getCollection()->print()) );

                } else if ( clct->typeId == VarType::DOUBLE ) { 

                    tuple->get<double>()->setAt( getFromInput<double>( ui->valueInput->text() ),
                                                 getFromInput<int>( ui->index1Input->text() ) );
                    item->setText( QString::fromStdString( tuple->getCollection()->print()) );

                } else if ( clct->typeId == VarType::PERSON ) {

                    tuple->get<Person>()->setAt( getFromInput<Person>( ui->valueInput->text() ), 
                                                 getFromInput<int>( ui->index1Input->text() ) );
                    item->setText( QString::fromStdString( tuple->getCollection()->print()) );

                } else if ( clct->typeId == VarType::STRING ) {

                    tuple->get<std::string>()->setAt( getFromInput<std::string>( ui->valueInput->text() ), 
                                                      getFromInput<int>( ui->index1Input->text() ) );
                    item->setText( QString::fromStdString( tuple->getCollection()->print()) );

                } else { throw Exception( Exception::ErrorCode::INVALID_TYPE ); }
            } else {
                ICollectionTuple<Sequence, int, double, Person, std::string>* changedTuple;
                if ( clct->typeId == VarType::INT ) {

                    auto seq = tuple->get<int>()->setAtImmutable( getFromInput<int>( ui->valueInput->text() ),
                                                                  getFromInput<int>( ui->index1Input->text() ) );
                    changedTuple = WrapFactory::enwrap<int>(seq);
                    addToList(*changedTuple);

                } else if ( clct->typeId == VarType::DOUBLE ) {

                    auto seq = tuple->get<double>()->setAtImmutable( getFromInput<double>( ui->valueInput->text() ), 
                                                                     getFromInput<int>( ui->index1Input->text() ) );
                    changedTuple = WrapFactory::enwrap<double>(seq);
                    addToList(*changedTuple);

                } else if ( clct->typeId == VarType::PERSON ) {

                    auto seq = tuple->get<Person>()->setAtImmutable( getFromInput<Person>( ui->valueInput->text() ),
                                                                     getFromInput<int>( ui->index1Input->text() ) );
                    changedTuple = WrapFactory::enwrap<Person>(seq);
                    addToList(*changedTuple);

                } else if ( clct->typeId == VarType::STRING ) {

                    auto seq = tuple->get<std::string>()->setAtImmutable( getFromInput<std::string>( ui->valueInput->text() ), 
                                                                          getFromInput<int>( ui->index1Input->text() ) );
                    changedTuple = WrapFactory::enwrap<std::string>(seq);
                    addToList(*changedTuple);

                } else {
                    throw Exception( Exception::ErrorCode::INVALID_TYPE ); 
                }
            }
        } else {
            throw Exception( Exception::ErrorCode::INVALID_SELECTION ) ;
        }
    } catch ( Exception &ex ) {
        showError(Exception(ex));
    }
}

void MainWindow::on_insertAtBtn_clicked()
{
    try {
        QListWidgetItem* item = ui->seqList->item(ui->seqList->currentRow());
        if (item) {
            auto tuple = item->data(Qt::UserRole).value<SequenceTuple>();
            auto clct = tuple->getCollection();

            if ( ui->index1Input->text().isEmpty() || ui->valueInput->text().isEmpty() ) {
                throw Exception( Exception::ErrorCode::INVALID_INPUT );
            }

            if (!ui->isImmutable->isChecked()) {
                if ( clct->typeId == VarType::INT ) { 

                    tuple->get<int>()->insertAt( getFromInput<int>( ui->valueInput->text() ), 
                                                 getFromInput<int>( ui->index1Input->text() ) );

                } else if ( clct->typeId == VarType::DOUBLE ) { 

                    tuple->get<double>()->insertAt( getFromInput<double>( ui->valueInput->text() ), 
                                                    getFromInput<int>( ui->index1Input->text() ) );

                } else if ( clct->typeId == VarType::PERSON ) {

                    tuple->get<Person>()->insertAt( getFromInput<Person>( ui->valueInput->text() ), 
                                                    getFromInput<int>( ui->index1Input->text() ) );

                } else if ( clct->typeId == VarType::STRING ) {

                    tuple->get<std::string>()->insertAt( getFromInput<std::string>( ui->valueInput->text() ), 
                                                         getFromInput<int>( ui->index1Input->text() ) );

                } else { throw Exception( Exception::ErrorCode::INVALID_TYPE ); }

                item->setText( QString::fromStdString( tuple->getCollection()->print()) );

            } else {
                ICollectionTuple<Sequence, int, double, Person, std::string>* changedTuple;
                if ( clct->typeId == VarType::INT ) {

                    auto seq = tuple->get<int>()->insertAtImmutable( getFromInput<int>( ui->valueInput->text() ), 
                                                                     getFromInput<int>( ui->index1Input->text() ) );
                    changedTuple = WrapFactory::enwrap<int>(seq);
                    addToList(*changedTuple);

                } else if ( clct->typeId == VarType::DOUBLE ) {

                    auto seq = tuple->get<double>()->insertAtImmutable( getFromInput<double>( ui->valueInput->text() ),
                                                                        getFromInput<int>( ui->index1Input->text() ) );
                    changedTuple = WrapFactory::enwrap<double>(seq);
                    addToList(*changedTuple);

                } else if ( clct->typeId == VarType::PERSON ) {

                    auto seq = tuple->get<Person>()->insertAtImmutable( getFromInput<Person>( ui->valueInput->text() ), 
                                                                        getFromInput<int>( ui->index1Input->text() ) );
                    changedTuple = WrapFactory::enwrap<Person>(seq);
                    addToList(*changedTuple);

                } else if ( clct->typeId == VarType::STRING ) {

                    auto seq = tuple->get<std::string>()->insertAtImmutable( getFromInput<std::string>( ui->valueInput->text() ), 
                                                                             getFromInput<int>( ui->index1Input->text() ) );
                    changedTuple = WrapFactory::enwrap<std::string>(seq);
                    addToList(*changedTuple);

                } else {
                    throw Exception( Exception::ErrorCode::INVALID_TYPE ); 
                }
            }
        } else {
            throw Exception( Exception::ErrorCode::INVALID_SELECTION ) ;
        }
    } catch ( Exception &ex ) {
        showError(Exception(ex));
    }
}

void MainWindow::on_swapBtn_clicked() 
{
    try {
        QListWidgetItem* item = ui->seqList->item(ui->seqList->currentRow());
        if (item) {
            auto tuple = item->data(Qt::UserRole).value<SequenceTuple>();
            auto clct = tuple->getCollection();

            if ( ui->index1Input->text().isEmpty() || ui->index2Input->text().isEmpty() ) {
                throw Exception( Exception::ErrorCode::INVALID_INPUT );
            }

            if (!ui->isImmutable->isChecked()) {
                if ( clct->typeId == VarType::INT ) { 

                    tuple->get<int>()->swap( getFromInput<int>( ui->index1Input->text() ), 
                                             getFromInput<int>( ui->index2Input->text() ) );
                    item->setText( QString::fromStdString( tuple->getCollection()->print()) );

                } else if ( clct->typeId == VarType::DOUBLE ) { 

                    tuple->get<double>()->swap( getFromInput<int>( ui->index1Input->text() ), 
                                                getFromInput<int>( ui->index2Input->text() ) );
                    item->setText( QString::fromStdString( tuple->getCollection()->print()) );

                } else if ( clct->typeId == VarType::PERSON ) {

                    tuple->get<Person>()->swap( getFromInput<int>( ui->index1Input->text() ), 
                                                getFromInput<int>( ui->index2Input->text() ) );
                    item->setText( QString::fromStdString( tuple->getCollection()->print()) );

                } else if ( clct->typeId == VarType::STRING ) {

                    tuple->get<std::string>()->swap( getFromInput<int>( ui->index1Input->text() ), 
                                                     getFromInput<int>( ui->index2Input->text() ) );
                    item->setText( QString::fromStdString( tuple->getCollection()->print()) );

                } else { throw Exception( Exception::ErrorCode::INVALID_TYPE ); }
            } else {
                ICollectionTuple<Sequence, int, double, Person, std::string>* changedTuple;
                if ( clct->typeId == VarType::INT ) {

                    auto seq = tuple->get<int>()->swapImmutable( getFromInput<int>( ui->index1Input->text() ), 
                                                                 getFromInput<int>( ui->index2Input->text() ) );
                    changedTuple = WrapFactory::enwrap<int>(seq);
                    addToList(*changedTuple);

                } else if ( clct->typeId == VarType::DOUBLE ) {

                    auto seq = tuple->get<double>()->swapImmutable( getFromInput<int>( ui->index1Input->text() ), 
                                                                    getFromInput<int>( ui->index2Input->text() ) );
                    changedTuple = WrapFactory::enwrap<double>(seq);
                    addToList(*changedTuple);

                } else if ( clct->typeId == VarType::PERSON ) {

                    auto seq = tuple->get<Person>()->swapImmutable( getFromInput<int>( ui->index1Input->text() ), 
                                                                    getFromInput<int>( ui->index2Input->text() ) );
                    changedTuple = WrapFactory::enwrap<Person>(seq);
                    addToList(*changedTuple);

                } else if ( clct->typeId == VarType::STRING ) {

                    auto seq = tuple->get<std::string>()->swapImmutable( getFromInput<int>( ui->index1Input->text() ), 
                                                                         getFromInput<int>( ui->index2Input->text() ) );
                    changedTuple = WrapFactory::enwrap<std::string>(seq);
                    addToList(*changedTuple);

                } else {
                    throw Exception( Exception::ErrorCode::INVALID_TYPE ); 
                }
            }
        } else {
            throw Exception( Exception::ErrorCode::INVALID_SELECTION ) ;
        }
    } catch ( Exception &ex ) {
        showError(Exception(ex));
    }
}

void MainWindow::addToList( ICollectionTuple<Sequence, int, double, Person, std::string>& tuple ) 
{
    QListWidgetItem* item = new QListWidgetItem( QString::fromStdString( tuple.getCollection()->print() ));
    item->setData(Qt::UserRole, QVariant::fromValue(&tuple));
    ui->seqList->addItem(item);
}

void MainWindow::on_copyBtn_clicked() 
{
    try {
        QListWidgetItem* item = ui->seqList->item(ui->seqList->currentRow());
        if (item) {
            auto tuple = item->data(Qt::UserRole).value<SequenceTuple>();
            auto clct = tuple->getCollection();

            ICollectionTuple<Sequence, int, double, Person, std::string>* tupleCopy;
            if ( clct->typeId == VarType::INT ) {

                auto seq = tuple->get<int>()->clone();
                tupleCopy = WrapFactory::enwrap<int>(seq);
                addToList(*tupleCopy);

            } else if ( clct->typeId == VarType::DOUBLE ) {

                auto seq = tuple->get<double>()->clone();
                tupleCopy = WrapFactory::enwrap<double>(seq);
                addToList(*tupleCopy);

            } else if ( clct->typeId == VarType::PERSON ) {

                auto seq = tuple->get<Person>()->clone();
                tupleCopy = WrapFactory::enwrap<Person>(seq);
                addToList(*tupleCopy);

            } else if ( clct->typeId == VarType::STRING ) {

                auto seq = tuple->get<std::string>()->clone();
                tupleCopy = WrapFactory::enwrap<std::string>(seq);
                addToList(*tupleCopy);

            } else {
                throw Exception( Exception::ErrorCode::INVALID_TYPE ); 
            }
        } else {
            throw Exception( Exception::ErrorCode::INVALID_SELECTION ) ;
        }
    } catch ( Exception &ex ) {
        showError(Exception(ex));
    }
}

void MainWindow::on_concatBtn_clicked()
{
    try {
        QList<QListWidgetItem*> selectedItems = ui->seqList->selectedItems();
        if ( selectedItems.size() != 2 ) {
            throw Exception( Exception::ErrorCode::INVALID_SELECTION ) ;
        } 
        QListWidgetItem* item1 = selectedItems[0];
        QListWidgetItem* item2 = selectedItems[1];

        if ( item1 && item2 ) {
            auto tuple1 = item1->data(Qt::UserRole).value<SequenceTuple>();
            auto tuple2 = item2->data(Qt::UserRole).value<SequenceTuple>();

            auto clct1 = tuple1->getCollection();
            auto clct2 = tuple2->getCollection();

            if (!ui->isImmutable->isChecked()) {
                if ( clct1->typeId != clct2->typeId ) {
                    throw Exception( Exception::ErrorCode::INVALID_TYPE );
                } else if ( clct1->typeId == VarType::INT ) {

                    auto seq = tuple1->get<int>()->concat(*tuple2->get<int>());
                    item1->setText( QString::fromStdString( tuple1->getCollection()->print()) );

                } else if ( clct1->typeId == VarType::DOUBLE ) {

                    auto seq = tuple1->get<double>()->concat(*tuple2->get<double>());
                    item1->setText( QString::fromStdString( tuple1->getCollection()->print()) );

                } else if ( clct1->typeId == VarType::PERSON ) {

                    auto seq = tuple1->get<Person>()->concat(*tuple2->get<Person>());
                    item1->setText( QString::fromStdString( tuple1->getCollection()->print()) );

                } else if ( clct1->typeId == VarType::STRING ) {

                    auto seq = tuple1->get<std::string>()->concat(*tuple2->get<std::string>());
                    item1->setText( QString::fromStdString( tuple1->getCollection()->print()) );

                } else {
                    throw Exception( Exception::ErrorCode::INVALID_TYPE );
                }
            } else {
                ICollectionTuple<Sequence, int, double, Person, std::string>* changedTuple;

                if ( clct1->typeId != clct2->typeId ) {
                    throw Exception( Exception::ErrorCode::INVALID_TYPE );
                } else if ( clct1->typeId == VarType::INT ) {

                    auto seq = tuple1->get<int>()->concat(*tuple2->get<int>());
                    changedTuple = WrapFactory::enwrap<int>(seq);
                    addToList(*changedTuple);

                } else if ( clct1->typeId == VarType::DOUBLE ) {

                    auto seq = tuple1->get<double>()->concat(*tuple2->get<double>());
                    changedTuple = WrapFactory::enwrap<double>(seq);
                    addToList(*changedTuple);

                } else if ( clct1->typeId == VarType::PERSON ) {

                    auto seq = tuple1->get<Person>()->concat(*tuple2->get<Person>());
                    changedTuple = WrapFactory::enwrap<Person>(seq);
                    addToList(*changedTuple);

                } else if ( clct1->typeId == VarType::STRING ) {

                    auto seq = tuple1->get<std::string>()->concat(*tuple2->get<std::string>());
                    changedTuple = WrapFactory::enwrap<std::string>(seq);
                    addToList(*changedTuple);

                } else {
                    throw Exception( Exception::ErrorCode::INVALID_TYPE );
                }
            }
        } else {
            throw Exception( Exception::ErrorCode::INVALID_SELECTION ) ;
        }
    } catch ( Exception &ex ) {
        showError(Exception(ex));
    }
}

void MainWindow::on_subSeqBtn_clicked() 
{
    try {
        QListWidgetItem* item = ui->seqList->item(ui->seqList->currentRow());
        if (item) {
            auto tuple = item->data(Qt::UserRole).value<SequenceTuple>();
            auto clct = tuple->getCollection();

            if ( ui->index1Input->text().isEmpty() || ui->index2Input->text().isEmpty() ) {
                throw Exception( Exception::ErrorCode::INVALID_INPUT );
            }

            ICollectionTuple<Sequence, int, double, Person, std::string>* subTuple;

            if ( clct->typeId == VarType::INT ) {

                auto seq = tuple->get<int>()->getSubSequence( getFromInput<int>( ui->index1Input->text() ), 
                                                              getFromInput<int>( ui->index2Input->text() ) );
                subTuple = WrapFactory::enwrap<int>(seq);
                addToList(*subTuple);

            } else if ( clct->typeId == VarType::DOUBLE ) {

                auto seq = tuple->get<double>()->getSubSequence( getFromInput<int>( ui->index1Input->text() ), 
                                                                 getFromInput<int>( ui->index2Input->text() ) );
                subTuple = WrapFactory::enwrap<double>(seq);
                addToList(*subTuple);

            } else if ( clct->typeId == VarType::PERSON ) {

                auto seq = tuple->get<Person>()->getSubSequence( getFromInput<int>( ui->index1Input->text() ),
                                                                 getFromInput<int>( ui->index2Input->text() ) );
                subTuple = WrapFactory::enwrap<Person>(seq);
                addToList(*subTuple);

            } else if ( clct->typeId == VarType::STRING ) {

                auto seq = tuple->get<std::string>()->getSubSequence( getFromInput<int>( ui->index1Input->text() ), 
                                                                      getFromInput<int>( ui->index2Input->text() ) );
                subTuple = WrapFactory::enwrap<std::string>(seq);
                addToList(*subTuple);

            } else {
                throw Exception( Exception::ErrorCode::INVALID_TYPE ); 
            }
        } else {
            throw Exception( Exception::ErrorCode::INVALID_SELECTION ) ;
        }
    } catch ( Exception &ex ) {
        showError(Exception(ex));
    }
}

template <typename T>
T MainWindow::getFromInput( const QString &input ) const {
    try {
        auto str = input.toStdString();
        std::istringstream in(str);
        T value;

        in >> value;

        return value;
    } catch ( Exception &ex ) {
        throw Exception( Exception::ErrorCode::INVALID_INPUT );
    }
}

void MainWindow::on_mapBtn_clicked() 
{
    try {
        QListWidgetItem* item = ui->seqList->item(ui->seqList->currentRow());
        if (item) {
            auto tuple = item->data(Qt::UserRole).value<SequenceTuple>();
            auto clct = tuple->getCollection();

            if (!ui->isImmutable->isChecked()) {
                if ( clct->typeId == VarType::INT ) {

                    if ( ui->mapFunc->currentIndex() == mapFunc::DOUBLEUP ) {
                        tuple->get<int>()->map( doubleUp );
                    } else if ( ui->mapFunc->currentIndex() == mapFunc::SIGN ) {
                        tuple->get<int>()->map( sign );
                    }

                } else if ( clct->typeId == VarType::DOUBLE ) {

                    if ( ui->mapFunc->currentIndex() == mapFunc::DOUBLEUP ) {
                        tuple->get<double>()->map( doubleUp );
                    } else if ( ui->mapFunc->currentIndex() == mapFunc::SIGN ) {
                        tuple->get<double>()->map( sign );
                    }

                } else if ( clct->typeId == VarType::PERSON ) {

                    if ( ui->mapFunc->currentIndex() == mapFunc::DOUBLEUP ) {
                        tuple->get<Person>()->map( doubleUp );
                    } else if ( ui->mapFunc->currentIndex() == mapFunc::SIGN ) {
                        tuple->get<Person>()->map( sign );
                    }

                } else if ( clct->typeId == VarType::STRING ) {

                    if ( ui->mapFunc->currentIndex() == mapFunc::DOUBLEUP ) {
                        tuple->get<std::string>()->map( doubleUp );
                    } else if ( ui->mapFunc->currentIndex() == mapFunc::SIGN ) {
                        tuple->get<std::string>()->map( sign );
                    }

                } else {
                    throw Exception( Exception::ErrorCode::INVALID_TYPE ); 
                }

                item->setText( QString::fromStdString( tuple->getCollection()->print()) );
                
            } else {
                ICollectionTuple<Sequence, int, double, Person, std::string>* changedTuple;
                if ( clct->typeId == VarType::INT ) {

                    if ( ui->mapFunc->currentIndex() == mapFunc::DOUBLEUP ) {

                        auto seq = tuple->get<int>()->mapImmutable( doubleUp );
                        changedTuple = WrapFactory::enwrap<int>(seq);
                        addToList(*changedTuple);

                    } else if ( ui->mapFunc->currentIndex() == mapFunc::SIGN ) {

                        auto seq = tuple->get<int>()->mapImmutable( sign );
                        changedTuple = WrapFactory::enwrap<int>(seq);
                        addToList(*changedTuple);
                        
                    }

                } else if ( clct->typeId == VarType::DOUBLE ) {

                    if ( ui->mapFunc->currentIndex() == mapFunc::DOUBLEUP ) {

                        auto seq = tuple->get<double>()->mapImmutable( doubleUp );
                        changedTuple = WrapFactory::enwrap<double>(seq);
                        addToList(*changedTuple);

                    } else if ( ui->mapFunc->currentIndex() == mapFunc::SIGN ) {

                        auto seq = tuple->get<double>()->mapImmutable( sign );
                        changedTuple = WrapFactory::enwrap<double>(seq);
                        addToList(*changedTuple);
                        
                    }

                } else if ( clct->typeId == VarType::PERSON ) {

                    if ( ui->mapFunc->currentIndex() == mapFunc::DOUBLEUP ) {

                        auto seq = tuple->get<Person>()->mapImmutable( doubleUp );
                        changedTuple = WrapFactory::enwrap<Person>(seq);
                        addToList(*changedTuple);

                    } else if ( ui->mapFunc->currentIndex() == mapFunc::SIGN ) {

                        auto seq = tuple->get<Person>()->mapImmutable( sign );
                        changedTuple = WrapFactory::enwrap<Person>(seq);
                        addToList(*changedTuple);
                        
                    }

                } else if ( clct->typeId == VarType::STRING ) {

                    if ( ui->mapFunc->currentIndex() == mapFunc::DOUBLEUP ) {

                        auto seq = tuple->get<std::string>()->mapImmutable( doubleUp );
                        changedTuple = WrapFactory::enwrap<std::string>(seq);
                        addToList(*changedTuple);

                    } else if ( ui->mapFunc->currentIndex() == mapFunc::SIGN ) {

                        auto seq = tuple->get<std::string>()->mapImmutable( sign );
                        changedTuple = WrapFactory::enwrap<std::string>(seq);
                        addToList(*changedTuple);
                        
                    }

                } else {
                    throw Exception( Exception::ErrorCode::INVALID_TYPE ); 
                }
            }
        } else {
            throw Exception( Exception::ErrorCode::INVALID_SELECTION ) ;
        }
    } catch ( Exception &ex ) {
        showError(Exception(ex));
    }
}