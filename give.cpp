/*

http://www.decisionsciencenews.com/2017/06/19/counterintuitive-problem-everyone-room-keeps-giving-dollars-random-others-youll-never-guess-happens-next/

g++ -Wall -Wextra -std=c++11 give.cpp -o give


give 4 1 1 999 4

give 100 100 1 0
give 100 100 1 999
give 100 100 1 999 4


*/

// Includes
    #include <stdio.h> // printf
    #include <math.h>
    #include <random>


// Consts
    int PEOPLE = 10;
    int MONEY  =  1;
    int GIFT   =  1;
    int TIME   = 60;
    int RICH   = PEOPLE*MONEY;

// Types

    enum Verbose_e
    {
          SHOW_CASH = 1 << 0 // 1 Show everyone's wealth
        , SHOW_XFER = 1 << 1 // 2 Show who transfered money from/to
        , SHOW_MOST = 1 << 2 // 4 Show person with most money
    };

// Globals
    std::mt19937 gRandom;
    int *gaWealth  = 0;
    int  giTime    = 0;
    int  gbVerbose = 0;
    int  gnWidth   = 3; // log(RICH)/log(10)

// ========================================================================
void dumpWealth()
{
    printf( "%d: ", giTime );
    for( int iPerson = 0; iPerson < PEOPLE; iPerson++ )
    {
        if( (iPerson % 25) == 0 )
            printf( "\n%*s", gnWidth, "" );
        printf( "$%*d, ", gnWidth, gaWealth[ iPerson ] );
    }
    printf( "\n" );
}

// ========================================================================
int findRichest()
{
    int iRichest = 0;
    int nWealth  = gaWealth[ iRichest ];

    for( int iPerson = 1; iPerson < PEOPLE; iPerson++ )
        if( nWealth < gaWealth[ iPerson ] )
        {
            nWealth  = gaWealth[ iPerson ];
            iRichest = iPerson;
        }

    return iRichest;
}


// ========================================================================
void dumpRichest()
{
    int iRichest = findRichest();
    int nWealth  = gaWealth[ iRichest ];

    printf( "%d: $$$%d @ %d\n", giTime, nWealth, iRichest );
}

// ========================================================================
void initWealth()
{
    int iAmount   = MONEY;
    gaWealth = new int[ PEOPLE ];

//iAmount = 0; // DEBUG: PLOT

    for( int iPerson = 0; iPerson < PEOPLE; iPerson++ )
        gaWealth[ iPerson ] = iAmount;

//gaWealth[0] = RICH; // DEBUG: PLOT
}

// ========================================================================
void moveWealth()
{
    for( int iPerson = 0; iPerson < PEOPLE; iPerson++ )
    {
        int iAmount   = GIFT;
        int iReceiver = PEOPLE;

        if( gaWealth[ iPerson ] >= iAmount )
        {

            while( iReceiver >= PEOPLE )
                iReceiver = gRandom() % PEOPLE;

            if( gbVerbose & SHOW_XFER )
                printf( "[%d]-$%d->[%d]\n", iPerson, iAmount, iReceiver );

            gaWealth[ iPerson   ] -= iAmount;
            gaWealth[ iReceiver ] += iAmount;

            if( gaWealth[ iReceiver ] >= RICH )
                printf( "%d: Player %d hogg'n da money!\n", giTime, iReceiver );
        }
    }
}

// ========================================================================
void plotWealth( int iTotalWealth = 0 )
{
    const int iRichest = findRichest();
    const int nWealth  = gaWealth[ iRichest ];

    /* */ int  nLines  = 10;
    /* */ int  iLines  = nLines;

    const char aPlot[] = ".X";
    /* */ int  aTotal[] =
    {
          RICH
        , PEOPLE
        , nWealth
    };
    const int  TOTAL = aTotal[ iTotalWealth ];

    const char *aDescription[] =
    {
          "Absolute Total Wealth"
        , "Relative Start Wealth"
        , "Relative Wealthiest  "
    };

    printf( "\n===== %s ===== [%d]: $%d\n"
        , aDescription[ iTotalWealth ]
        , iRichest
        , nWealth
    );

    while( iLines --> 0 )
    {
        printf( "%3d%% ", (100 * iLines) / nLines );

        for( int iPerson = 0; iPerson < PEOPLE; iPerson++ )
        {
            int percent = (nLines * gaWealth[ iPerson]) / TOTAL;
            int bDot    = percent > iLines;
            printf( "%c", aPlot[ bDot ] );
        }
        printf( "\n" );
    }

    printf( "---- " );
    for( int iPerson = 0; iPerson < PEOPLE; iPerson++ )
        if( (iPerson % 4) == 0)
            printf( "%-3d ", iPerson );
    printf( " player \n" );
}

// ========================================================================
int main( const int nArgs, const char *aArgs[] )
{
    std::random_device seed;     // Will be used to obtain a seed for the random number engine
//  gRandom.seed(      seed() ); // Standard mersenne_twister_engine seeded with rd()
    gRandom.seed( 1 );

    if( nArgs > 1 ) PEOPLE    = atoi( aArgs[ 1 ] );
    if( nArgs > 2 ) MONEY     = atoi( aArgs[ 2 ] );
    if( nArgs > 3 ) GIFT      = atoi( aArgs[ 3 ] );
    if( nArgs > 4 ) TIME      = atoi( aArgs[ 4 ] );
    if( nArgs > 5 ) gbVerbose = atoi( aArgs[ 5 ] );

    RICH = PEOPLE * MONEY;
    gnWidth = (int) floor( log( RICH ) / log( 10.0 ) );
printf( "Width: %d\n", gnWidth );

    initWealth();
    dumpWealth();

    for( giTime = 0; giTime < TIME; giTime++ )
    {
        moveWealth();
        if( gbVerbose & SHOW_CASH )
            dumpWealth();

        if( gbVerbose & SHOW_MOST )
            dumpRichest();

//      if( gbVerbose & ~SHOW_CASH )
//          printf( "\n" );
    }

    dumpWealth();
    plotWealth(0); // Relative to starting money
    plotWealth(1); // Relative to total    money
    plotWealth(2); // Relative to richest  person

    delete [] gaWealth;

    return 0;
}
