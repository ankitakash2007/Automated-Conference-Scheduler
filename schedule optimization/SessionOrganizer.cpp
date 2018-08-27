/* 
 * File:   SessionOrganizer.cpp
 * Author: Kapil Thakkar
 * 
 */

#include "SessionOrganizer.h"
#include "Util.h"
#include <vector>

SessionOrganizer::SessionOrganizer ( )
{
    parallelTracks = 0;
    papersInSession = 0;
    sessionsInTrack = 0;
    processingTimeInMinutes = 0;
    tradeoffCoefficient = 1.0;
}

SessionOrganizer::SessionOrganizer ( string filename )
{
    readInInputFile ( filename );
    conference = new Conference ( parallelTracks, sessionsInTrack, papersInSession );
}

void SessionOrganizer::organizePapers ( )
{
    int paperCounter = 0;
    for ( int i = 0; i < conference->getSessionsInTrack ( ); i++ )
    {
        for ( int j = 0; j < conference->getParallelTracks ( ); j++ )
        {
            for ( int k = 0; k < conference->getPapersInSession ( ); k++ )
            {
                conference->setPaper ( j, i, k, paperCounter );
                paperCounter++;
            }
        }
    }
}





void SessionOrganizer::randomOrganizePapers ( )
{
    int paperCounter = 0;
      std::srand ( unsigned ( std::time(0) ) );
      std::vector<int> myvector;

      // set some values:
      int papers = conference->getPapersInSession ( );
      int sessions = conference->getSessionsInTrack ( );
      int parallel = conference->getParallelTracks ( );
      for (int i=0; i<papers*sessions*parallel; i++) myvector.push_back(i); // 1 2 3 4 5 6 7 8 9

      // using built-in random generator:
      std::random_shuffle ( myvector.begin(), myvector.end() );
      // cout<<"random "<<myvector[0]<<myvector[1]<<myvector[2]<<myvector[3]<<"\n";
      // using myrandom:
      // std::random_shuffle ( myvector.begin(), myvector.end(), std::rand()%(papers*sessions*parallel));
        // for(int j = 0; j<papers*sessions*parallel;j++){
        //     cout<<myvector[j]<<" ";
        // }
        // cout<<"\n";
    for ( int i = 0; i < conference->getSessionsInTrack ( ); i++ )
    {
        for ( int j = 0; j < conference->getParallelTracks ( ); j++ )
        {
            for ( int k = 0; k < conference->getPapersInSession ( ); k++ )
            {
                conference->setPaper ( j, i, k, myvector[paperCounter] );
                paperCounter++;
            }
        }
    }
}

void SessionOrganizer::greedy ()
{
    bool flag = true;
    double max = 0;
     tempconference = new Conference();

     for (int i = 0; i < 1; i++){
            if (i==0) organizePapers();
            else randomOrganizePapers();
            bool flag = true;
        while(flag == true){
            vector<int> one_change(8);
            one_change = changeOnePosition();
           // cout<<"greedy "<<one_change[0]<<one_change[1]<<one_change[2]<<one_change[3]<<one_change[4]<<one_change[5]<<one_change[6]<<one_change[7]<<"\n";
            flag = false;
            for(auto x: one_change) {
                if (x != 0){
                    flag = true;
                }
            }
            if(flag==true){
                conference->setPaper ( one_change[0], one_change[1], one_change[2], one_change[7] );
                conference->setPaper ( one_change[3], one_change[4], one_change[5], one_change[6] );
                Track tmpTrack1 = conference->getTrack ( 0 );
                Session tmpSession1 = tmpTrack1.getSession ( 0 );
                //cout<<"local search "<<one_change[0]<<one_change[1]<<one_change[2]<<one_change[3]<<one_change[4]<<one_change[5]<<one_change[6]<<one_change[7]<<"\n";
             
            }
            else{
                double score = scoreOrganization();
                // if(score>max){
                //     cout<<"updated";
                //     *tempconference = *conference;
                //     max = score;
                //     //cout<<tempconference->scoreOrganization()<<" temp "<<conference->scoreOrganization();
                // }
                cout<<"score for i: "<<i<<" is "<<score<<"\n";
            }
        }

     }
    // *conference = *tempconference;
}

vector<int> SessionOrganizer::changeOnePosition ()
{
    double max_diff = 0;
    //cout<<max_score<<" initial \n";
    int z;
    double prescore, postscore;
    vector<int> change1(8,0);
    for ( int i = 0; i < conference->getParallelTracks ( ); i++ )
    {
        Track tmpTrack1 = conference->getTrack ( i );
        for ( int j = 0; j < tmpTrack1.getNumberOfSessions  ( ); j++ )
        {
            Session tmpSession1 = tmpTrack1.getSession ( j );
            for ( int k = 0; k < tmpSession1.getNumberOfPapers ( ); k++ )
            {


                for ( int l = 0; l < conference->getParallelTracks ( ); l++ )
                {
                    Track tmpTrack2 = conference->getTrack ( l );
                    if(l>i) z=j;
                    else z=j+1;
                    for ( int m = z; m < tmpTrack2.getNumberOfSessions ( ); m++ )
                    {
                        Session tmpSession2 = tmpTrack2.getSession ( m );
                        for ( int n = 0; n < tmpSession2.getNumberOfPapers ( ); n++ )
                        {                
                            //cout <<tmpSession1.getNumberOfPapers()<<" for k "<<k<<"\n";
                            //cout <<tmpSession2.getNumberOfPapers()<<" for n "<<n<<"\n";
                            int q;
                            prescore = PseudoScore(i,j,k)+PseudoScore(l,m,n) ; 
                            int index1 = tmpSession1.getPaper (k);
                            int index2 = tmpSession2.getPaper (n);
                            //cout<<i<<" "<<j<<" "<<k<<" "<<l<<" "<<m<<" "<<n<<"\n";
                            tmpSession1.setPaper(k,index2);
                            tmpSession2.setPaper(n,index1);
                            postscore = PseudoScore(i,j,k)+PseudoScore(l,m,n);

                            if(postscore-prescore>max_diff){
                                change1[0] = i;change1[1] = j; change1[2] = k;
                                change1[3] = l;change1[4] = m; change1[5] = n;
                                change1[6] = index1; change1[7] = index2;
                                max_diff = postscore-prescore;
                            }
                           // cout<< "max_score "<<max_score<<" tempscore "<<tempscore<<" i,j,k,l,m,n "<<i<<j<<k<<l<<m<<n<<"\n";
                            tmpSession1.setPaper(k,index1);
                            tmpSession2.setPaper(n,index2);
                        }
                    }
                }
                //int index1 = tmpSession1.getPaper ( k );

                // Get competing papers.
                // for ( int l = i; l < conference->getParallelTracks ( ); l++ )
                // {
                //     Track tmpTrack2 = conference->getTrack ( l );
                //     Session tmpSession2 = tmpTrack2.getSession ( j );
                //     for ( int m = 0; m < tmpSession2.getNumberOfPapers ( ); m++ )
                //     {
                //         int index2 = tmpSession2.getPaper ( m );
                //         score2 += distanceMatrix[index1][index2];
                //     }
                // }
            }
        }
    }
    if( max_diff == 0){
        cout << "no better path";
    }
    return change1;
}

void SessionOrganizer::readInInputFile ( string filename )
{
    vector<string> lines;
    string line;
    ifstream myfile ( filename.c_str () );
    if ( myfile.is_open ( ) )
    {
        while ( getline ( myfile, line ) )
        {
            //cout<<"Line read:"<<line<<endl;
            lines.push_back ( line );
        }
        myfile.close ( );
    }
    else
    {
        cout << "Unable to open input file";
        exit ( 0 );
    }

    if ( 6 > lines.size ( ) )
    {
        cout << "Not enough information given, check format of input file";
        exit ( 0 );
    }

    processingTimeInMinutes = atof ( lines[0].c_str () );
    papersInSession = atoi ( lines[1].c_str () );
    parallelTracks = atoi ( lines[2].c_str () );
    sessionsInTrack = atoi ( lines[3].c_str () );
    tradeoffCoefficient = atof ( lines[4].c_str () );

    int n = lines.size ( ) - 5;
    double ** tempDistanceMatrix = new double*[n];
    for ( int i = 0; i < n; ++i )
    {
        tempDistanceMatrix[i] = new double[n];
    }


    for ( int i = 0; i < n; i++ )
    {
        string tempLine = lines[ i + 5 ];
        string *elements = new string[n];
        splitString ( tempLine, " ", elements, n );

        for ( int j = 0; j < n; j++ )
        {
            tempDistanceMatrix[i][j] = atof ( elements[j].c_str () );
        }
    }
    distanceMatrix = tempDistanceMatrix;

    int numberOfPapers = n;
    int slots = parallelTracks * papersInSession*sessionsInTrack;
    if ( slots != numberOfPapers )
    {
        cout << "More papers than slots available! slots:" << slots << " num papers:" << numberOfPapers << endl;
        exit ( 0 );
    }
}

double** SessionOrganizer::getDistanceMatrix ( )
{
    return distanceMatrix;
}

void SessionOrganizer::printSessionOrganiser ( char * filename)
{
    conference->printConference ( filename);
}

double SessionOrganizer::scoreOrganization ( )
{
    // Sum of pairwise similarities per session.
    double score1 = 0.0;
    for ( int i = 0; i < conference->getParallelTracks ( ); i++ )
    {
        Track tmpTrack = conference->getTrack ( i );
        for ( int j = 0; j < tmpTrack.getNumberOfSessions ( ); j++ )
        {
            Session tmpSession = tmpTrack.getSession ( j );
            for ( int k = 0; k < tmpSession.getNumberOfPapers ( ); k++ )
            {
                int index1 = tmpSession.getPaper ( k );
                for ( int l = k + 1; l < tmpSession.getNumberOfPapers ( ); l++ )
                {
                    int index2 = tmpSession.getPaper ( l );
                    score1 += 1 - distanceMatrix[index1][index2];
                }
            }
        }
    }

    // Sum of distances for competing papers.
    double score2 = 0.0;
    for ( int i = 0; i < conference->getParallelTracks ( ); i++ )
    {
        Track tmpTrack1 = conference->getTrack ( i );
        for ( int j = 0; j < tmpTrack1.getNumberOfSessions ( ); j++ )
        {
            Session tmpSession1 = tmpTrack1.getSession ( j );
            for ( int k = 0; k < tmpSession1.getNumberOfPapers ( ); k++ )
            {
                int index1 = tmpSession1.getPaper ( k );

                // Get competing papers.
                for ( int l = i + 1; l < conference->getParallelTracks ( ); l++ )
                {
                    Track tmpTrack2 = conference->getTrack ( l );
                    Session tmpSession2 = tmpTrack2.getSession ( j );
                    for ( int m = 0; m < tmpSession2.getNumberOfPapers ( ); m++ )
                    {
                        int index2 = tmpSession2.getPaper ( m );
                        score2 += distanceMatrix[index1][index2];
                    }
                }
            }
        }
    }
    // cout<<score1<<" "<<score2<<"\n";
    double score = score1 + tradeoffCoefficient*score2;
    return score;
}

double SessionOrganizer::PseudoScore(int i, int j, int k){
    // Sum of pairwise similarities per session.
            Track tmpTrack = conference->getTrack ( i );
                        Session tmpSession = tmpTrack.getSession ( j );

    double score1 = 0.0;
        int index1 = tmpSession.getPaper ( k );
        for ( int l = k + 1; l < tmpSession.getNumberOfPapers ( ); l++ )
        {
            int index2 = tmpSession.getPaper ( l );
            score1 += 1 - distanceMatrix[index1][index2];
        }

    // Sum of distances for competing papers.
    double score2 = 0.0;
        Track tmpTrack1 = conference->getTrack ( i );
            Session tmpSession1 = tmpTrack1.getSession ( j );

                // Get competing papers.
                for ( int l = i + 1; l < conference->getParallelTracks ( ); l++ )
                {
                    Track tmpTrack2 = conference->getTrack ( l );
                    Session tmpSession2 = tmpTrack2.getSession ( j );
                    for ( int m = 0; m < tmpSession2.getNumberOfPapers ( ); m++ )
                    {
                        int index2 = tmpSession2.getPaper ( m );
                        score2 += distanceMatrix[index1][index2];
                    }
                }
    double score = score1 + tradeoffCoefficient*score2;
    return score;
}
