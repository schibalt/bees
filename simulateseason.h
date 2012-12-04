#ifndef SIMULATESEASON_H
#define SIMULATESEASON_H

#include <QObject>
#include <QThread>
#include <QDebug>

#include <iostream>
#include <algorithm>

#include "bee.h"
#include "hive.h"
#include "normal.hpp"

using namespace std;

class WorkerBee : public QObject
{
        Q_OBJECT

    public:
        explicit WorkerBee(QObject* parent = 0);

        //colony generation
        const vector<Bee >* getBees();
        Hive getHive();
        void setGenesisMembers(QThread&, int , QSize , bool);

        //field generation
        const double** getField();

        void setFieldGenMembers(
            QThread&,
            int,
            int,
            int,
            int,
            bool,
            bool,
            bool
        );

        const double** getFoxholes();
        void setFitnessEvalMembers(QThread&,double);

        //site selection
        void setSiteSelectionMembers(QThread& , int , int);
       void setRecruitmentMembers(QThread& , double ,double ,double );
       const vector<vector< Bee* > >* getEliteNeighborhoods();
       const vector<vector< Bee* > >* getPriorityNeighborhoods();

       //neighborhood fitness evaluation

       void setNeighborFitEvalMembs(QThread&);

       void newGenMembers(QThread&);

    private:
        void disconnectEverything(QThread&);

        //colony generation
        int _population;
        vector<Bee > _bees;
        Hive _hive;
        QSize _fieldDims;
        bool _add;

        void setHive();

        //field generation
        int _maxima;
        int _foxholeParam;
        int _bound;
        int _power;
        const static int _DIMENSIONS = 2;
        double** _foxholes;
        double** _field;
        bool _deterministic;
        bool _d1 ;
        bool _d2;

        double foxHelper(int*);

        //fitness evaluation
        int _sites;
        int _eliteSites;

           double _distancePenalty;
        //runtime
        int _seasonLength;

        //step 1
        vector<Bee* > _priorityBees;
        vector<Bee* > _eliteBees;

        //step 2
        void evaluateFitnesses(vector<Bee >);
        void moveToSite(vector<Bee* > );

        double _randomCut;
        double _deltaLambda ;
        double _deltaPhi;
        double _eliteWeight;
        double _priorityWeight;
        vector<vector< Bee* > > _eliteNeighborhoods;
        vector<vector< Bee* > > _priorityNeighborhoods;

        //step 3

    signals:
        void quitBeeGenThread();
        void beesGenerated();

        void foxholesGenerated();
        void quitFieldGenThread();
        void fieldGenerated();

        void quitFitEvalThread();
        void fitnessesEvaluated();

        void quitSiteSelectThread();
        void sitesSelected();

        void quitRecruitmentThread();
        void beesRecruited();

        void quitNeighborFitEvalThread();
        void neighborFitsEval();

        void quitRegenThread();
        void regenerated();

    public slots:
        void generate();

        void foxholes();

        void evaluateFitnesses();

        void selectSites();
        void recruit();

        void evalNeighborFits();

        void regenerate();

    private slots:
        void computeField();

};

#endif // SIMULATESEASON_H
