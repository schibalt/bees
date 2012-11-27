
#include "algorithmmodel.h"
#include <normal.hpp>

AlgorithmModel::AlgorithmModel()
{
}

vector<Bee > AlgorithmModel::genesis(const int population, const QSize fieldDims, bool add)
{
    if (!add)
        _bees.clear();

    time_t rawtime;
    time(&rawtime);

    // seed the rand num gen with the current time
    srand(rawtime);

    for (int i = 0; i < population; i++)
    {
        Bee newBee(fieldDims);
        //newBee.setPoint(QPoint(xPos, yPos));
        //cout << "making bee " << i << endl;

        _bees.push_back(newBee);
    }

    return _bees;
}

void AlgorithmModel::computeField(double** foxholes)
{
    ofstream indexFilestream;
    char indexFilename[] = "indices.txt";
    indexFilestream.open(indexFilename, ios::out);

    const int fieldWidth = _fieldDims.width();
    const int fieldHeight = _fieldDims.height();

    double** field = new double*[fieldHeight];

    for (int i = 0; i < fieldHeight; ++i)
    {
        field[i] = new double[fieldWidth];

        for (int j = 0; j < fieldWidth; j++)
            field[i][j] = 0;
    }

    double upperLeftVal;
    double upperRightVal;
    double lowerLeftVal;
    double lowerRightVal;

    int subMatWidth = _fieldDims.width() / (_SHEKEL_DIMENSION * 2);
    int subMatHeight = _fieldDims.height() / (_SHEKEL_DIMENSION * 2);

    //int subMatWidth = _fieldDims.width() / (_SHEKEL_DIMENSION - 1);
    //int subMatHeight = _fieldDims.height() / (_SHEKEL_DIMENSION - 1);

    float xLeftRatio;
    float yUpRatio;
    float xRightRatio;
    float yDownRatio;

    //double upperXDifference;
    //double lowerXDifference;

    double weightedUpperLeftValue;
    double weightedLowerLeftValue;
    double weightedUpperRightValue;
    double weightedLowerRightValue;

    //double yDifference;

    int iIdx;
    int jIdx;

    for (int k = 1; k <= (_SHEKEL_DIMENSION * 2); k++)
    {
        for (int l = 1; l <= (_SHEKEL_DIMENSION * 2); l++)
        {
            upperLeftVal = foxholes[k - 1][l - 1];
            upperRightVal = foxholes[k - 1][l];
            lowerLeftVal = foxholes[k][l - 1];
            lowerRightVal = foxholes[k][l];

            //upperXDifference = upperRightVal - upperLeftVal;
            //lowerXDifference = lowerRightVal - lowerLeftVal;

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

                    //yDifference = weightedLowerValue - weightedUpperValue;

                    iIdx = (k - 1) * subMatHeight + i;
                    jIdx = (l - 1) * subMatWidth + j;

                    //field[iIdx][jIdx]  = weightedLowerValue + yDifference * yRatio;
                    field[iIdx][jIdx]  = weightedUpperLeftValue + weightedLowerLeftValue
                                         + weightedUpperRightValue + weightedLowerRightValue;
                }
            }
        }
    }

    _field =  const_cast<const double**>(field);

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
}

const double** AlgorithmModel::getField()
{
    return  _field;
}

void AlgorithmModel::setHive(const QSize fieldDims)
{
    _fieldDims = fieldDims;
    _hive = Hive(fieldDims);
}

Hive AlgorithmModel::getHive()
{
    return _hive;
}

void AlgorithmModel::setBees(vector<Bee > bees)
{
    this->_bees = bees;
}

const vector<Bee > AlgorithmModel::getBees()
{
    cout << "returning bee vector size " << _bees.size() << endl;
    return  _bees;
}

double** AlgorithmModel::foxholes()
{
    //function y = foxholes(x, noPause)

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

    //const int testMatDimension = 10;
    const int shekelArraySize = 2 * _SHEKEL_DIMENSION + 1;

    double** F = new double*[shekelArraySize];

    for (int i = 0; i < shekelArraySize; ++i)
    {
        F[i] = new double[shekelArraySize];

        for (int j = 0; j < shekelArraySize; j++)
            F[i][j] = 0;
    }

    //inst array (default size = 21)
    int x [shekelArraySize];
    cout << "the shekel array has " << shekelArraySize << " elements" << endl;

    int value;

    //init array (default -10:1:10)
    for (int idx = 0; idx < shekelArraySize; idx++)
    {
        value = idx - 10;
        x[idx] = value;
    }

    time_t rawtime;
    time(&rawtime);

    // seed the rand num gen with the current time
    srand(rawtime);

    for (int m = 0; m < shekelArraySize; m++)
        for (int n = 0; n < shekelArraySize; n++)
        {
            int constants [2];
            constants[0] = x[m];
            constants[1] = x[n];
            F[m][n] = foxHelper(constants);
        }
    return F;
}

double AlgorithmModel::foxHelper(int* x)
{
    float sum = 0.0;
    int seed = rand();
    float normal;
    float tmp;
    float tmp2 = 0.0;

    for (int i = 0; i < pow(_SHEKEL_DIMENSION, 2); i++)
    {
         tmp = 0.0;

        for (int j = 0; j < 2; j++)
        {
            normal = r4_normal(0, 15, seed);
            tmp2 = tmp2 + (x[1] - normal);
            //% h = x(2) - a(i, j);
            //tmp = sum + tmp2 * tmp2;
        }
        tmp = tmp + 1 / (i + tmp2);
        //normal = r4_normal(0.5, 0.2, seed);
    }
    sum = 1 / (0.002 + tmp);

    return sum;

    //if (nargin == 1 || ~noPause)
    //    pause(0.05);
}
