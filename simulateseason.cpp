#include "simulateseason.h"

WorkerBee::WorkerBee(QObject* parent) :
    QObject(parent)
{
}

void WorkerBee::disconnectEverything(QThread& thread)
{
    //colony/hive generation
    disconnect(&thread, SIGNAL(started()), this, SLOT(genesis()));
    //disconnect(this, SIGNAL(beesGenerated()), &thread, SLOT(quit()));

    //field generation
    disconnect(&thread, SIGNAL(started()), this, SLOT(foxholes()));
    //disconnect(this, SIGNAL(fieldGenerated()), &thread, SLOT(quit()));
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

    int subMatWidth = _fieldDims.width() / (_foxholeNumber * 2);
    int subMatHeight = _fieldDims.height() / (_foxholeNumber * 2);

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

    const int shekelArraySize = 2 * _foxholeNumber + 1;

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

                    iIdx = (k - 1) * subMatHeight + i;
                    jIdx = (l - 1) * subMatWidth + j;

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
    emit fieldGenerated();
}

void WorkerBee::setFieldGenMembers(
    QThread& thread,
    int foxholeNumber,
    int shekelMaxima
)
{
    disconnectEverything(thread);

    //when the thread it started generate the bees
    connect(&thread, SIGNAL(started()), SLOT(foxholes()));

    _foxholeNumber = foxholeNumber;
    _shekelMaxima = shekelMaxima;
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

    /*
    //inst/init result matrix
    double** F = new double*[foxholeMatDim];

    for (int i = 0; i < foxholeMatDim; ++i)
    {
        F[i] = new double[foxholeMatDim];

        //for (int j = 0; j < shekelArraySize; j++)
        //   F[i][j] = 0;
    }
    */

    //set array size (default 21)
    const int foxholeMatDim = 2 * _foxholeNumber + 1;

    //inst/init result matrix
    _foxholes = new double*[foxholeMatDim];

    for (int i = 0; i < foxholeMatDim; ++i)
    {
        _foxholes[i] = new double[foxholeMatDim];

        //for (int j = 0; j < shekelArraySize; j++)
        //   F[i][j] = 0;
    }

    //inst array (default size = 21)
    int x [foxholeMatDim];
    cout << "the shekel array has " << foxholeMatDim << " elements" << endl;

    int value;

    //init array (default -10:1:10)
    for (int idx = 0; idx < foxholeMatDim; idx++)
    {
        value = idx - _foxholeNumber;
        //cout << 'x' << idx << " is " << value << endl;
        x[idx] = value;
    }

    time_t rawtime;
    time(&rawtime);

    // seed the rand num gen with the current time
    srand(rawtime);
    int seed = rand();

    double foxhole;
    int xpair [2];

    for (int m = 0; m < foxholeMatDim; m++)
        for (int n = 0; n < foxholeMatDim; n++)
        {

            xpair[0] = x[m];
            xpair[1] = x[n];

            foxhole = foxHelper(xpair, seed);
            //F[m][n] = foxhole;
            _foxholes[m][n] = foxhole;
        }
    emit foxholesGenerated();
}

double WorkerBee::foxHelper(int* x, int seed)
{
    float tmp = 0.0;
    float normal;
    float tmp2;

    //static int a[2][25] = {{-32, -16, 0, 16, 32, -32, -16, 0, 16, 32, -32, -16, 0, 16, 32, -32, -16, 0, 16, 32, -32, -16, 0, 16, 32},
    //                     {-32, -32, -32, -32, -32, -32, -32, -32, -32, -32, -32, -32, 0, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32}};

    for (int i = 0; i < _shekelMaxima; i++)
    {
        tmp2 = 0.0;

        for (int j = 0; j < 2; j++)
        {
            seed = rand();
            normal = r4_normal(0, 15, seed);
            //cout << "0:10 " << normal << endl;

            tmp2 += pow(x[1] - normal, 2);
            //tmp2 += pow(x[j] - normal, 6);//good
            //tmp2 += pow(x[j] - a[j][i], 2);
        }

        tmp += (1 / (tmp2 + i));
    }

    //normal = r4_normal(0, 250, rand());

    //cout << "0:1 " << normal << endl;

    return 1 / (.002 + tmp);
    //return -1 * (1 / (.002 + tmp));
    //return normal * (.002 + tmp);
}
