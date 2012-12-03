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

    //select sites
    disconnect(&thread, SIGNAL(started()), this, SLOT(selectSites()));
    disconnect(this, SIGNAL(quitSiteSelectThread()), &thread, SLOT(quit()));

    //recruit bees
    disconnect(&thread, SIGNAL(started()), this, SLOT(recruit()));
    disconnect(this, SIGNAL(quitRecruitmentThread()), &thread, SLOT(quit()));
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

void WorkerBee::setFieldGenMembers(
    QThread& thread,
    int foxholes,
    int maxima,
    int bound,
    int power,
    bool deterministic
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
    _deterministic = deterministic;
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
    const int maximaSqrt = sqrt(_maxima);

    double constA [_maxima][2];
    double a0 [maximaSqrt];

    int maxSqrtHalf = maximaSqrt / 2;

    for (int constIdx = 0; constIdx < maximaSqrt; constIdx++)
        a0[constIdx] = ((constIdx - maxSqrtHalf) * 16);

    for (int idx = 0; idx < maximaSqrt; idx++)
        for (int subidx = 0; subidx < maximaSqrt; subidx++)
        {
            int cell = (maximaSqrt * idx) + subidx;
            constA[cell][0] = a0[subidx];
            constA[cell][1] = a0[idx];
        }

    float tmp = 0.0;
    float tmp2;

    double aval;
    int seed = rand();
    float uniform;
    const int matdim = (2 * _foxholeParam) + 1;

    for (int j = 0; j < _maxima; j++)
    {
        tmp2 = 0.0;

        for (int i = 0; i < _DIMENSIONS; i++)
        {
            uniform = r4_uniform_01(seed);
            //qDebug() << "uniform is " << uniform;

            if (_deterministic)
                aval = constA[j][i];
            else
                aval = (uniform * matdim) - _foxholeParam;

            //qDebug() << "aval is " << aval;

            tmp2 += pow(x[i] - aval, _power);
        }
        uniform = _maxima * r4_uniform_01(seed);
        // qDebug() << "uniform is " << uniform;

        if (_deterministic)
            tmp += (1 / (j + tmp2));
        else
            tmp += (1 / (tmp2 + uniform));
    }

    if (_deterministic)
    {
        return -(1 / (.002 + tmp));
        return (1 / (.002 + tmp));
        return 119.998 - tmp;
    }
    else
        return tmp * 10;
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

void WorkerBee::setFitnessEvalMembers(QThread& thread)
{
    disconnectEverything(thread);

    //when the thread it started generate the bees
    connect(&thread, SIGNAL(started()), this, SLOT(evaluateFitnesses()));
    connect(this, SIGNAL(quitFitEvalThread()), &thread, SLOT(quit()));
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

    /*
    inc = 0;

    foreach(Bee bee, _bees)
    {
         qDebug() << "bee " << inc << " fitness is " << bee.getFitness();

        ++inc;
    }
    */

    emit quitFitEvalThread();
    emit fitnessesEvaluated();
}

void WorkerBee::setSiteSelectionMembers(QThread& thread, int sites, int eliteSites)
{
    disconnectEverything(thread);

    connect(&thread, SIGNAL(started()), this, SLOT(selectSites()));
    connect(this, SIGNAL(quitSiteSelectThread()), &thread, SLOT(quit()));

    _sites = sites;
    _eliteSites = eliteSites;
}

void WorkerBee::selectSites()
{
    _priorityBees.clear();
    _eliteBees.clear();

    int eliteOffset =  1 + _eliteSites;
    int priorityOffset = 1 + _sites;

    for (vector<Bee >::iterator i = _bees.end() - eliteOffset; i != _bees.end() - priorityOffset; --i)
    {
        (*i).setRole(Bee::PRIORITY);
        _priorityBees.push_back((*i));
        qDebug() << "bee with fitness " << (*i).getFitness() << " is at a priority site";
    }

    for (vector<Bee >::iterator i = _bees.end() - 1; i != _bees.end() - eliteOffset; --i)
    {
        (*i).setRole(Bee::ELITE);
        _eliteBees.push_back((*i));
        qDebug() << "bee with fitness " << (*i).getFitness() << " is at an elite site";
    }
    emit quitSiteSelectThread();
    emit sitesSelected();
}

void WorkerBee::setRecruitmentMembers
(
    QThread& thread,
    double randomCut,
    double deltaLambda,
    double deltaPhi
)
{
    disconnectEverything(thread);

    _randomCut = randomCut;
    _deltaLambda = deltaLambda;
    _deltaPhi = deltaPhi;
    _eliteWeight = 2;
    _priorityWeight = 1;

    connect(&thread, SIGNAL(started()), this, SLOT(recruit()));
    connect(this, SIGNAL(quitRecruitmentThread()), &thread, SLOT(quit()));
}

void WorkerBee::recruit()
{
    //convert to raiot 0:1
    double scoutRatio = _randomCut / 100;

    //set the actual number of scouts
    int scouts = _bees.size() * scoutRatio;

    int elites = _eliteBees.size();
    int prioritized = _priorityBees.size();

    //recruits + priority bees + elite bees
    int remainder = _bees.size() - scouts - elites - prioritized;

    //weight the numbers of bees at pri. + elite sites
    double eliteWeight = elites * _eliteWeight;
    double priorityWeight = prioritized * _priorityWeight;

    //set ratios 0:1
    double eliteRatio = (eliteWeight / (eliteWeight + priorityWeight));
    double priorityRatio = 1 - eliteRatio;

    //set bees per site type
    int eliteRecruits = remainder * eliteRatio;
    int priorityRecruits = remainder * priorityRatio;

    //set bees per site
    int perPrioritySite = priorityRecruits / prioritized;
    int perEliteSite = eliteRecruits / elites;

    //account for remainder
    int extraElites = eliteRecruits - (elites * perEliteSite);
    int extraPrioritized = priorityRecruits - (prioritized * perPrioritySite);

    //add to scout pool
    scouts += extraElites;
    scouts += extraPrioritized;

    //seems unecessary
    //perEliteSite -= extraElites;
    //perPrioritySite -= extraPrioritized;

    int bee = 0;

    _eliteNeighborhoods.clear();
    _priorityNeighborhoods.clear();
    Bee* neighborBee;

    for (int eliteHood = 0; eliteHood < elites; eliteHood++)
    {
        vector<Bee* > newEliteNeighborhood;
        newEliteNeighborhood.push_back(&_eliteBees[eliteHood]);
        ++bee;

        for (int eliteNeighbor = 0; eliteNeighbor < perEliteSite; eliteNeighbor++)
        {
            neighborBee = &_bees[bee];
            neighborBee->setRole(Bee::RECRUIT);
            newEliteNeighborhood.push_back(neighborBee);
            ++bee;
        }
        _eliteNeighborhoods.push_back(newEliteNeighborhood);
        moveToSite(_eliteNeighborhoods.back());
    }

    for (int priorityHood = 0; priorityHood < prioritized; priorityHood++)
    {
        vector<Bee* > newPriorityNeighborhood;
        newPriorityNeighborhood.push_back(&_priorityBees[priorityHood]);
        ++bee;

        for (int prioritizedNeighbor = 0; prioritizedNeighbor < perPrioritySite; prioritizedNeighbor++)
        {
            neighborBee = &_bees[bee];
            neighborBee->setRole(Bee::RECRUIT);
            newPriorityNeighborhood.push_back(neighborBee);
            ++bee;
        }
        _priorityNeighborhoods.push_back(newPriorityNeighborhood);
        moveToSite(_priorityNeighborhoods.back());
    }

    emit quitRecruitmentThread();
    emit beesRecruited();
}

void WorkerBee::moveToSite(vector<Bee* > neighborhood)
{
    int fieldWidth = _fieldDims.width();
    int fieldHeight = _fieldDims.height();

    int deltaLambda = fieldWidth * _deltaLambda;
    deltaLambda /= 100;
    int deltaPhi = fieldHeight * _deltaPhi;
    deltaPhi /= 100;

    Bee* mainBee = neighborhood.front();

    int boundaryWest = mainBee->getPoint().x() - deltaLambda;

    if (boundaryWest < 0)
        boundaryWest = 0;

    int boundaryEast = mainBee->getPoint().x() + deltaLambda;

    if (boundaryEast >= fieldWidth)
        boundaryEast = fieldWidth - 1;

    int boundaryNorth = mainBee->getPoint().y() - deltaPhi;

    if (boundaryNorth < 0)
        boundaryNorth = 0;

    int boundarySouth = mainBee->getPoint().y() + deltaPhi;

    if (boundarySouth >= fieldHeight)
        boundarySouth = fieldHeight - 1;

    deltaLambda = boundaryEast - boundaryWest;
    deltaPhi = boundarySouth - boundaryNorth;

    qDebug() << "evaluating neighborhood fitnesses";

    int hiveJ = _hive.getPoint().x();
    int hiveI = _hive.getPoint().y();

    double fieldPtQuality;
    double distance;
    double fitness;
    //int inc = 0;

    int newX;
    int newY;

    int seed = rand();

    for (vector<Bee* >::iterator i = neighborhood.begin(); i != neighborhood.end(); ++i)
    {
        Bee* bee = (*i);

        newX = boundaryWest + (deltaLambda * r4_uniform_01(seed));

        newY = boundaryNorth + (deltaPhi * r4_uniform_01(seed));

        bee->setPoint(QPoint(newX, newY));

        distance = sqrt(pow(newX - hiveJ, 2) + pow(newY - hiveI, 2));
        fieldPtQuality = _field[newY][newX];

        fitness = fieldPtQuality / distance;
        //qDebug() << "bee " << inc << " fitness is " << fitness;
        //++inc;

        bee->setFitness(fitness);
    }

    //save for next step
    //sort(neighborhood.begin(), neighborhood.end());
}

const vector<vector< Bee* > >* WorkerBee::getEliteNeighborhoods()
{
    return const_cast<const vector<vector< Bee* > >* >(&_eliteNeighborhoods);
}

const vector<vector< Bee* > >* WorkerBee::getPriorityNeighborhoods()
{
    return const_cast<const vector<vector< Bee* > >* >(&_priorityNeighborhoods);
}
