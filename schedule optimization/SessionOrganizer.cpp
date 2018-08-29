/* 
 * File:   SessionOrganizer.cpp
 * Author: Kapil Thakkar
 * 1. initially one by one placing
 * 2. initally took state which has most score in random
 * 3. separate for big and small values.
 * 4. 
 */

#include "SessionOrganizer.h"
#include "Util.h"
#include <ctime>
#include <vector>

SessionOrganizer::SessionOrganizer ( )
{
    parallelTracks = 0;
    papersInSession = 0;
    sessionsInTrack = 0;
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


void SessionOrganizer::initialstart()
{
    double score =0;
    std::vector<int> used;
    for ( int i = 0; i < conference->getSessionsInTrack ( ); i++ )
    {
        for ( int j = 0; j < conference->getParallelTracks ( ); j++ )
        {
            for ( int k = 0; k < conference->getPapersInSession ( ); k++ )
            {
                if(i==j&&j==k&&k==0){
                    conference->setPaper ( j, i, k, 0);
                    used.push_back(0);
                }
                else{
                    double tempscore1=0;
                    double tempscore2=0;
                    double tempmax =0;
                    int putvalue=0;
                    for(int l=0; l<parallelTracks*sessionsInTrack*papersInSession; l++){
                        tempscore2=tempscore1=0;
                        bool flag=true;
                        for(int p=0;p<used.size();p++){
                            int tp= used[p];
                            if(tp==l){
                                flag=false;
                            }
                        }

                        if(flag==true){//l is not in vector
                            if(j==0&&k==0){
                                putvalue=l;
                                l=parallelTracks*sessionsInTrack*papersInSession;
                            }
                            for(int m=0; m<k;m++){
                                Track tmpTrack = conference->getTrack ( j );
                                Session tmpSession = tmpTrack.getSession ( i );
                                int index1 = tmpSession.getPaper ( m );   
                                tempscore1+=1-distanceMatrix[index1][l];
                            }
                            for(int m=0;m<j;m++){
                                for(int n=0;n<conference->getPapersInSession();n++){
                                    Track tmpTrack = conference->getTrack ( m );
                                    Session tmpSession = tmpTrack.getSession ( i );
                                    int index1 = tmpSession.getPaper ( n );   
                                    tempscore2+=distanceMatrix[index1][l];                                    
                                }
                            }
                            double temptempscore=tempscore1+tradeoffCoefficient*tempscore2;
                            //cout<<"temptemp: "<<temptempscore<<"tempmax: "<<tempmax<<" l "<<l<<"\n";
                            if(tempmax<temptempscore){
                                tempmax=temptempscore;
                                putvalue=l;
                            }  
                            
                        }
                    }
                    //cout<<"setting i,j,k="<<i<<","<<j<<","<<k<<"by score: "<<score<<" putvalue: "<<putvalue<<"\n";
                    conference->setPaper(j,i,k,putvalue);
                    used.push_back(putvalue);
                    score+=tempmax;
                }

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
      for (int i=0; i<parallelTracks*sessionsInTrack*papersInSession; i++) myvector.push_back(i); // 1 2 3 4 5 6 7 8 9

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
    std::clock_t start;
    double duration;
    bool flag = true;
    double max = 0;
    // if(parallelTracks*sessionsInTrack*papersInSession>30){
    //     int maxbig=0;
    //     int tempbig=0;
    //     for(int i=0;i<100;i++){
    //         if(i!=0) randomOrganizePapers();
    //         tempbig=scoreOrganization();  
    //         cout<<"tempbig"<<tempbig<<"\n";

    //         if(tempbig>maxbig){
    //             maxbig=tempbig;
    //         }

    //     }
    // }
    for (int i = 0; i < 10&&duration<60; i++){
        if (i!=0) randomOrganizePapers();
        bool flag = true;
        while(flag == true && duration<60){
            vector<int> one_change(8);
            if(parallelTracks*sessionsInTrack*papersInSession>30){
                one_change = changeOnePositionbig();
            }
            else{
                one_change = changeOnePositionsmall();
            }
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
                double score = scoreOrganization();

                cout<<"                            score for i: "<<i<<" is "<<score<<"\n";

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
        duration = (std::clock()-start)/(double) CLOCKS_PER_SEC;
        }
     }

    // *conference = *tempconference;
}
vector<int> SessionOrganizer::changeOnePositionsmall ()
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
            }
        }
    }
    cout<<"small:"<<max_diff<<"\n";
    return change1;
}
// vector<int> SessionOrganizer::changeOnePositionbigrandom(){
//     bool flag=true;
//         vector<int> change1(8,0);

//     while(flag==true){
//         int i = rand()%parallelTracks;
//         int j = rand()%sessionsInTrack;
//         int k = rand()%papersInSession;
//         Track tmpTrack1 = conference->getTrack ( i );
//         Session tmpSession1 = tmpTrack1.getSession ( j );
//         int index1 = tmpSession1.getPaper (k);
//         double min = 2.0;
//         int minindex=0;
//         for(int l=0;l<parallelTracks*sessionsInTrack*papersInSession;l++){
//             if(distanceMatrix[index1][l]<min && index1!=l){
//                 min=distanceMatrix[index1][l];
//                 minindex=l;
//             }
//         }
//         //find position of minindex
//         for ( int l = 0; l < conference->getParallelTracks ( ); l++ )
//         {
//             Track tmpTrack2 = conference->getTrack ( l );
//             for ( int m = 0; m < tmpTrack2.getNumberOfSessions  ( ); m++ )
//             {
//                 Session tmpSession2 = tmpTrack2.getSession ( m );
//                 for ( int n = 0; n < tmpSession2.getNumberOfPapers ( ); n++ )
//                 {
//                     int index2 = tmpSession2.getPaper (n);
//                     if(index2==minindex){
//                         change1[0]=l;change1[1]=m;change1[2]=n;

//                     }
//                 }
//             }
//         }
//         double tempmax=0;
//         int k2=-1;
//         int maxindex;
//         //find max in same i and j
//         for ( int n = 0; n < tmpSession1.getNumberOfPapers ( ); n++ ){
//             int index2 = tmpSession1.getPaper (n);
//             if(distanceMatrix[index1][index2]>tempmax && n!=k){
//                 tempmax=distanceMatrix[index1][index2];
//                 k2=n;
//                 maxindex=index2;
//             }
//         }
//         if(k2==-1){
//             continue;
//         }
//         double max_diff = 0;
//         double prescore, postscore;
//         prescore = PseudoScore(change1[0],change1[1],change1[2])+PseudoScore(i,j,k2) ; 
//         //double bscore = scoreOrganization();
//         Track tmpTrack2 = conference->getTrack ( change1[0] );
//         Session tmpSession2 = tmpTrack2.getSession ( change1[1] );
//         tmpSession2.setPaper(change1[2],maxindex);  //on this
//         tmpSession1.setPaper(k2,minindex);
//         //double ascore = scoreOrganization();
//         // tmpSession1.setPaper(change1[2],maxindex);  //on this
//         // tmpSession2.setPaper(k2,minindex);
//         postscore = PseudoScore(change1[0],change1[1],change1[2])+PseudoScore(i,j,k2) ;
//         //cout<<ascore<<"after score"<<bscore<<" score,pseudo "<<postscore-prescore<<"\n";

//         if(postscore-prescore>max_diff &&(i!=change1[0]||j!=change1[1])){
//             change1[3] = i;change1[4] = j; change1[5] = k2;
//             change1[6] = minindex; change1[7] = maxindex ;
//             //max_diff = postscore-prescore;
//             flag=false;
//             cout<<"i,j,k,index: "<<i<<","<<j<<","<<k<<","<<index1<<" min i,j,k,index: "<<change1[0]<<","<<change1[1]<<","<<change1[2]<<","<<minindex<<" max i,j,k,index "<<i<<","<<j<<","<<k2<<","<<maxindex<<"\n";

//         }
//         else{
//             cout<<"no     "<<"i,j,k,index: "<<i<<","<<j<<","<<k<<","<<index1<<" min i,j,k,index: "<<change1[0]<<","<<change1[1]<<","<<change1[2]<<","<<minindex<<" max i,j,k,index "<<i<<","<<j<<","<<k2<<","<<maxindex<<"\n";
//         }
//          //bscore = scoreOrganization();

//         tmpSession2.setPaper(change1[2],minindex);
//         tmpSession1.setPaper(k2,maxindex);
//          //ascore = scoreOrganization();
//         //cout<<bscore<<"after score"<<ascore<<"\n";

//     }
//     return change1;
   

// }


vector<int> SessionOrganizer::changeOnePositionbig ()
{

    double max_diff = 0;
    //cout<<max_score<<" initial \n";
    int z;
    double score;
    score = scoreOrganization();
    double prescore, postscore;
    vector<int> change1(8,0);
    int count=0;
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
                            prescore = PseudoScore(i,j,k)+PseudoScore(l,m,n) ; 
                            int index1 = tmpSession1.getPaper (k);
                            int index2 = tmpSession2.getPaper (n);
                            //cout<<i<<" "<<j<<" "<<k<<" "<<l<<" "<<m<<" "<<n<<"\n";
                            tmpSession1.setPaper(k,index2);
                            tmpSession2.setPaper(n,index1);
                            postscore = PseudoScore(i,j,k)+PseudoScore(l,m,n);
                            //bool flag= false;
                            if(postscore-prescore>max_diff){
                                change1[0] = i;change1[1] = j; change1[2] = k;
                                change1[3] = l;change1[4] = m; change1[5] = n;
                                change1[6] = index1; change1[7] = index2;
                                max_diff = postscore-prescore;
                                count++;
                                //flag  = true;
                            }
                            else{
                                int tempq = postscore-prescore;
                                tmpSession1.setPaper(k,index1);
                                tmpSession2.setPaper(n,index2);
                            //    cout<<"not found max_diff: "<<max_diff<<" temp :"<<tempq<<"\n";
                            }
                           // cout<< "max_score "<<max_score<<" tempscore "<<tempscore<<" i,j,k,l,m,n "<<i<<j<<k<<l<<m<<n<<"\n";

                            if(count == 10){
                                i=j=k=l=m=n=2000;
                            }
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
    // if(max_diff==0){
    //     cout << "no better path";
    // }
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
    double score = score1 + tradeoffCoefficient*score2;
    return score;
}
double SessionOrganizer::PseudoScore(int i, int j, int k){
    // Sum of pairwise similarities per session.
            Track tmpTrack = conference->getTrack ( i );
                        Session tmpSession = tmpTrack.getSession ( j );

    double score1 = 0.0;
        int index1 = tmpSession.getPaper ( k );
        for ( int l = 0; l < tmpSession.getNumberOfPapers ( ); l++ )
        {
                int index2 = tmpSession.getPaper ( l );
                score1 += 1 - distanceMatrix[index1][index2];
            
        }

    // Sum of distances for competing papers.
    double score2 = 0.0;
        Track tmpTrack1 = conference->getTrack ( i );
            Session tmpSession1 = tmpTrack1.getSession ( j );

                // Get competing papers.
                for ( int l = 0; l < conference->getParallelTracks ( ); l++ )
                {
                    if(l!=i){
                        Track tmpTrack2 = conference->getTrack ( l );
                        Session tmpSession2 = tmpTrack2.getSession ( j );
                        for ( int m = 0; m < tmpSession2.getNumberOfPapers ( ); m++ )
                        {
                            int index2 = tmpSession2.getPaper ( m );
                            score2 += distanceMatrix[index1][index2];
                        }
                    }
                }
    double score = score1 + tradeoffCoefficient*score2;
    return score;
}

