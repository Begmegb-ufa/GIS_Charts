#ifndef GISCLASS_H
#define GISCLASS_H
#include <QList>

class GisClass
{ public:
    GisClass();
    GisClass(QList<QByteArray> &data, QList<QByteArray> &depths);
    void SetGis(QList<QByteArray>& data, QList<QByteArray> &depths);
    QList<double> GetGisData();
    QList<double> GetGisDepths();
    //double GetGisDataValue(int index);
    QString GetGisName();
    double GetMin();
    double GetMax();
    QString GetMinDepVal();
    QString GetMaxDepVal();
    bool GetOnliBlanksInData();

    void SetMin(QList<double> &list, double &min);
    void SetMax(QList<double> &list, double &max);

    ~GisClass();
private:
    QList<double> data;
    QList<double> depths;
    QString name;
    double min=std::numeric_limits<double>::max();
    int min_depth=std::numeric_limits<int>::max();
    double max=std::numeric_limits<double>::lowest();
    int max_depth=std::numeric_limits<int>::lowest();
    bool only_blanks_in_data=0;
};

#endif // GISCLASS_H
