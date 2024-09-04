#include "gisclass.h"
#include <QTextCodec>


GisClass::GisClass(QList<QByteArray> &data, QList<QByteArray> &depths)
{
SetGis(data,depths);
}

void GisClass::SetGis(QList<QByteArray> &data,QList<QByteArray> &depths)
{
    //настраиваем кодировку
    QTextCodec *winCodec = QTextCodec::codecForName("windows-1251");
    //записываем поле имя с правильной кодировкой
    this->name = winCodec->toUnicode(data[0]);

    //задаем данные GisClass::data и GisClass::depths
    int blanks_count=0;
    for (int i = 1; i < data.size(); ++i) {
        //проверка на наличие бланков, их не включаем в данные, если вся кривая бланк, то оставляем один в списке
        if(data[i]=="-999.25"){
            this->data.push_back(qQNaN());
            this->depths.push_back(qQNaN());
            ++blanks_count;
        } else {
            this->data.push_back(data[i].toDouble());
            this->depths.push_back(depths[i].toDouble());

            if (this->data[i-1]<this->min){
                this->min=this->data[i-1];
            }
            if (this->data[i-1]>this->max){
                this->max=this->data[i-1];
            }
            if (this->depths[i-1]<this->min_depth){
                this->min_depth=this->depths[i-1];
            }
            if (this->depths[i-1]>this->max_depth){
                this->max_depth=this->depths[i-1];
            }
        }
    }
    if (blanks_count==this->data.size()){
        this->only_blanks_in_data=1;
        min=max=min_depth=max_depth=0;

    }
    //задание min max
    //SetMin();

//    if (!this->data.empty()){
//    auto min_it = std::min_element(this->data.begin(),this->data.end());
//    this->min=*min_it;
//    auto max_it = std::max_element(this->data.begin(),this->data.end());
//    this->max=*max_it;
//    } else {
//        this->only_blanks_in_data=1;
//        this->max=0;
//        this->min=0;
//    }

}

QList<double> GisClass::GetGisData()
{
    return this->data;
}

QList<double> GisClass::GetGisDepths()
{
    return this->depths;
}

QString GisClass::GetGisName()
{
    return this->name;
}

double GisClass::GetMin()
{
    return this->min;
}

double GisClass::GetMax()
{
    return this->max;
}

QString GisClass::GetMinDepVal()
{
//    auto it=depths.begin();
//    return QString::number(*it);
    return QString::number(this->min_depth);
}

QString GisClass::GetMaxDepVal()
{
//    auto it=--depths.end();
//    return QString::number(*it);
    return QString::number(this->max_depth);
}

bool GisClass::GetOnliBlanksInData()
{
    return this->only_blanks_in_data;
}

void GisClass::SetMin(QList<double>& list,double &min)
{
//    min=list[0];
//    for(double temp:list)
//    {
//     if (temp<min&&temp!=qQNaN())
//         min=temp;
//    }

}

void GisClass::SetMax(QList<double> &list, double &max)
{
//    max=std::numeric_limits<double>::lowest();
//    int blanks_count=0;
//    for(double temp:list)
//    {if (temp==qQNaN()){
//         ++blanks_count;
//        }

//     if (temp>max&&temp!=qQNaN())
//         max=temp;
//    }

}
GisClass::~GisClass()
{

}
