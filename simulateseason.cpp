#include "simulateseason.h"

WorkerBee::WorkerBee(QObject* parent) :
    QObject(parent)
{
}

void WorkerBee::disconnectEverything(QThread& thread)
{
    //colony/hive generation
    disconnect(&thread, SIGNAL(started()), this, SLOT(genesis()));
    disconnect(this, SIGNAL(quitBeeGenThread()), &thread, SLOT(quit()));

    //field generation
    disconnect(&thread, SIGNAL(started()), this, SLOT(foxholes()));
    disconnect(this, SIGNAL(quitFieldGenThread()), &thread, SLOT(quit()));

    //evaluate fitnesses
    disconnect(&thread, SIGNAL(started()), this, SLOT(evaluateFitnesses()));
    disconnect(this, SIGNAL(quitFitEvalThread()), &thread, SLOT(quit()));
}

void WorkerBee::setGenesisMembers(
    QThread& thread,
    int population,
    QSize fieldDims,
    bool add
)
{
    disconnectEverything(thread);

    //when the thread it started generate the bees
    connect(&thread, SIGNAL(started()), SLOT(genesis()));
    connect(this, SIGNAL(quitBeeGenThread()), &thread, SLOT(quit()));

    _population = population;
    _fieldDims = fieldDims;
    _add = add;
}

void WorkerBee::setHive()
{
    _hive = Hive(_fieldDims);
}

Hive WorkerBee::getHive()
{
    return _hive;
}

const double** WorkerBee::getFoxholes()
{
    return  const_cast<const double**>(_foxholes);
}

const double** WorkerBee::getField()
{
    return  const_cast<const double**>(_field);
}

void WorkerBee::genesis()
{
    if (!_add)
        _bees.clear();

    time_t rawtime;
    time(&rawtime);

    // seed the rand num gen with the current time
    srand(rawtime);

    for (int i = 0; i < _population; i++)
    {
        Bee newBee(_fieldDims);
        //newBee.setPoint(QPoint(xPos, yPos));
        //qDebug() << "making bee " << i;

        _bees.push_back(newBee);
    }

    setHive();

    emit quitBeeGenThread();
    emit beesGenerated();
}

const vector<Bee >* WorkerBee::getBees()
{
    return const_cast<const vector<Bee >* >(&_bees);
}

void WorkerBee::computeField()
{
    ofstream indexFilestream;
    char indexFilename[] = "indices.txt";
    indexFilestream.open(indexFilename, ios::out);

    const int fieldWidth = _fieldDims.width();
    const int fieldHeight = _fieldDims.height();

    //double** field = new double*[fieldHeight];
    _field = new double*[fieldHeight];

    for (int i = 0; i < fieldHeight; ++i)
    {
        _field[i] = new double[fieldWidth];

        for (int j = 0; j < fieldWidth; j++)
            _field[i][j] = 0;
    }

    double upperLeftVal;
    double upperRightVal;
    double lowerLeftVal;
    double lowerRightVal;

    const int shekelArraySize = (2 * _foxholeParam) + 1;

    double subMatWidth = (double) _fieldDims.width() / (shekelArraySize - 1);
    double subMatHeight = (double) _fieldDims.height() / (shekelArraySize - 1);

    float xLeftRatio;
    float yUpRatio;
    float xRightRatio;
    float yDownRatio;

    double weightedUpperLeftValue;
    double weightedLowerLeftValue;
    double weightedUpperRightValue;
    double weightedLowerRightValue;

    int iIdx;
    int jIdx;

    for (int k = 1; k < shekelArraySize; k++)
        for (int l = 1; l < shekelArraySize; l++)
        {
            upperLeftVal = _foxholes[k - 1][l - 1];
            upperRightVal = _foxholes[k - 1][l];
            lowerLeftVal = _foxholes[k][l - 1];
            lowerRightVal = _foxholes[k][l];

            for (int i = 0; i < subMatHeight; i++)
            {
                yDownRatio = (float) i / subMatHeight;
                yUpRatio = 1 - yDownRatio;

                for (int j = 0; j < subMatWidth; j++)
                {
                    xRightRatio = (float) j / subMatWidth;
                    xLeftRatio = 1 - xRightRatio;

                    weightedUpperLeftValue = upperLeftVal * yUpRatio * xLeftRatio;
                    weightedLowerLeftValue = lowerLeftVal * xLeftRatio * yDownRatio;
                    weightedUpperRightValue = upperRightVal * yUpRatio * xRightRatio;
                    weightedLowerRightValue = lowerRightVal * yDownRatio * xRightRatio;

                    iIdx = ((k - 1) * subMatHeight) + i;
                    jIdx = ((l - 1) * subMatWidth) + j;

                    _field[iIdx][jIdx]  = weightedUpperLeftValue + weightedLowerLeftValue
                                          + weightedUpperRightValue + weightedLowerRightValue;
                }
            }
        }

    //_field =  const_cast<const double**>(field);

    ofstream fieldFilestream;
    char fieldFilename[] = "field.txt";
    fieldFilestream.open(fieldFilename, ios::out);

    for (int i = 0; i < fieldHeight; i++)
    {
        for (int j = 0; j < fieldWidth; j++)
        {
            fieldFilestream << _field[i][j] << " ";
        }
        fieldFilestream << endl;
    }
    fieldFilestream.close();

    emit quitFieldGenThread();
    emit fieldGenerated();
}

void WorkerBee::setFieldGenMembers(
    QThread& thread,
    int foxholes,
    int maxima,
    int bound,
    int power
)
{
    disconnectEverything(thread);

    //when the thread it started generate the bees
    connect(&thread, SIGNAL(started()), SLOT(foxholes()));
    connect(this, SIGNAL(quitFieldGenThread()), &thread, SLOT(quit()));

    _foxholeParam = foxholes;
    _maxima = maxima;
    _bound = bound;
    _power = power;
}

void WorkerBee::foxholes()
{
    /*
        %FOXHOLES  Evaluate Shekel's Foxholes function.
        %       Y = FOXHOLES(X) with a two-element input vector X evaluates Shekel's
        %       Foxholes function at X. The elements of X have to be from the interval
        %       [-65.536, 65.536]. The global minimum of this function is about
        %       0.998004 at X = [-32, -32].
        %
        %       FOXHOLES (without input arguments) displays the function as a contour
        %       plot.
        %
        %       Markus Buehren
        %       Last modified 03.02.2008
    */

    const int matdim = (2 * _foxholeParam) + 1;

    //inst/init result matrix
    _foxholes = new double*[matdim];

    for (int i = 0; i < matdim; ++i)
        _foxholes[i] = new double[matdim];

    int x [matdim];
   qDebug() << "the shekel array has " << matdim << " elements";

    int value;

    //init array (default -10:1:10)
    for (int idx = 0; idx < matdim; idx++)
    {
        value = idx - _foxholeParam;
        //qDebug() << 'x' << idx << " is " << value;
        x[idx] = value;
    }

    time_t rawtime;
    time(&rawtime);

    // seed the rand num gen with the current time
    srand(rawtime);

    //double foxhole;
    int xpair [2];

    for (int m = 0; m < matdim; m++)
        for (int n = 0; n < matdim; n++)
        {
         xpair[0] = x[m];
        xpair[1] = x[n];

        _foxholes[m][n] = foxHelper(xpair);
        }
    emit foxholesGenerated();
}

double WorkerBee::foxHelper(int* x)
{
    float tmp = 0.0;
    float tmp2;

    double aval;
    int seed = rand();
    float uniform;
    const int matdim = (2 * _foxholeParam) + 1;

    for (int i = 0; i < _maxima; i++)
    {
        tmp2 = 0.0;

        for (int j = 0; j < _DIMENSIONS; j++)
        {
            uniform = r4_uniform_01(seed);
            //qDebug() << "uniform is " << uniform;

            aval = (uniform * matdim) - _foxholeParam;
            //qDebug() << "aval is " << aval;
            tmp2 += pow(x[j] - aval, _power);
        }
        uniform = r4_uniform_01(seed);
       // qDebug() << "uniform is " << uniform;
        tmp += (1 / (tmp2 + uniform));
    }

    return tmp * 10;
}

void WorkerBee::setFitnessEvalMembers(QThread &thread, int sites, int eliteSites)
{
    disconnectEverything(thread);

    //when the thread it started generate the bees
    connect(&thread, SIGNAL(started()), this,SLOT(evaluateFitnesses()));
    connect(this, SIGNAL(quitFitEvalThread()), &thread, SLOT(quit()));

    _sites = sites;
    _eliteSites = eliteSites;
}

void WorkerBee::evaluateFitnesses()
{
    qDebug() << "evaluating fitnesses";
    int beeJ;
    int beeI;

    int hiveJ = _hive.getPoint().x();
    int hiveI = _hive.getPoint().y();

    double fieldPtQuality;
    double distance;
    double fitness;
    int inc = 0;

    for (vector<Bee >::iterator i = _bees.begin(); i != _bees.end(); ++i)
    {
        beeJ = (*i).getPoint().x();
        beeI = (*i).getPoint().y();
        distance = sqrt(pow(beeJ - hiveJ, 2) + pow(beeI - hiveI, 2));
        fieldPtQuality = _field[beeI][beeJ];

        fitness = fieldPtQuality / distance;
        //qDebug() << "bee " << inc << " fitness is " << fitness;

        (*i).setFitness(fitness);
        ++inc;
    }
    sort(_bees.begin(), _bees.end());

    inc = 0;

    foreach(Bee bee, _bees)
    {
        qDebug() << "bee " << inc << " fitness is " << bee.getFitness();

        ++inc;
    }
    /*
*/

    for (vector<Bee >::iterator i = _bees.end(); i != _bees.end() - _sites - 1; --i)
    {
        (*i).setRole(Bee::PRIORITY);
        qDebug() << "bee with fitness " << (*i).getFitness() << " is at a priority site";
    }

    for (vector<Bee >::iterator i = _bees.end(); i != _bees.end() - _eliteSites - 1; --i)
    {
        (*i).setRole(Bee::ELITE);
        qDebug() << "bee with fitness " << (*i).getFitness() << " is at an elite site";
    }
    emit quitFitEvalThread();
    emit fitnessesEvaluated();
}


void WorkerBee::evaluateFitnesses(vector<Bee > neighborhood)
{

}
