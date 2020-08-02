#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <conio.h>
// Cumali Toprak 180201090
// Berkay Efe Ozcan 180201080
struct locusNonRgb
{
    float x,y,z;
};
struct locusRgb
{

    float x,y,z;
    int r,g,b;

};
void convertBinary(char *filePath,char *outputpath, int rgb);
void compareLength(char *filePath,char *outputFilePath,int locusAmount,int rgb,char *fileName);
void distanceAverange(char *filePath,char *outputFilePath,int locusAmount,int rgb,char *fileName);
void cube(char *filePath,char *outputFilePath,int locusAmount,int rgb,char *fileName);
void sphereInfo(char *fileName, char *outputFile, int locusAmount, int rgb, float x1, float y1, float z1, float r1,char *fileNames); // secim 4
void controlNktFiles (int size,char nktFilePath[size][100] );
int controlInputForNum(char *str);
int nktFileAmountCounter();
int rowCounter (char *string_path);
int validateFileHeaders(char *inputFilePath,char *outputFilePath,int *rgb,int *givenLocusAmount,int *isBinary,char *fileName);
int validateLocus(char *fileName,char *outputFile, int givenLocusAmount,int rgb,int * locusAmount) ;

int main()
{
    const int nktFilesAmount = nktFileAmountCounter(); // dosya sayisini ogreniyoruz.
    char fileNames[nktFilesAmount][100];
    char menuFileNames[nktFilesAmount][100];
    char outputFile[100]="output.nkt";
    char selection[10];


    if(nktFilesAmount==0)  // .nkt uzantili nokta yoksa programdan cikilir.
    {
        printf("\n\tBulundugunuz dizinin icinde .nkt uzantili dosya bulunmamaktadir.Bu program .nktli dosyalar ile islem yapmaktadir.\n!!!");
        printf("Programdan cikmandan icin bir tusa basiniz.\n");
        getch();
        exit(1);

    }

    int isBinary;
    int isOk[nktFilesAmount];
    int locusAmount=0;
    int i, result1, result2;
    int arr_rgb[nktFilesAmount];
    int arr_locusAmount[nktFilesAmount];

    int rgb, givenLocusAmount=0;  //givenLocusAmount : header'da verilen nokta sayisi.
    FILE *fptr;


    int flag=0; // 1 e basilmadan diger isterlere basilmasini onledik.

    while(1)
    {
        system("cls"); // her isterden sonra menu ekran temizleniyor.
        printf("****************************************************************\n");
        printf("*\t1-Dosya kontrolu                                       *\n");
        printf("*\t2-Birbirine en yakin ve en uzak noktalari hesaplar     *\n");
        printf("*\t3-Kup bilgileri                                        *\n");
        printf("*\t4-Kure bilgileri                                       *\n");
        printf("*\t5-Nokta uzakliklarinin ortalamasi                      *\n");
        printf("*\t6-Programdan cikis                                     *\n");
        printf("****************************************************************\n\n");

        printf("Seciminiz:");
        scanf("%s",selection); // isterler icin input aliniyor.

        while(1)
        {
            while(!controlInputForNum(selection))
            {
                printf("Seciminizi duzgun formatta giriniz!!!\n");
                scanf("%s",selection);
            }
            if(atoi(selection)>7  || atoi(selection)<=0)
            {
                printf("Gecersiz secim!!!\n");
                printf("Tekrar giriniz : ");
                scanf("%s",selection);
            }
            else
                break;
        }


        int isError =0; // tum dosyalarin uyumlu olup olmadigini kontrol icin tanimladik.
        /* menuFileNames i kullanmamizin sebebi convertedTo diye baslayan asciiye cevrilmis dosyalarin ismini
        menude gostermemek. */

        switch(atoi(selection))
        {

        case 1: // ister 1
        {
            FILE *fptr=fopen(outputFile,"w"); // output dosyasi onceden varsa icerigi siliniyor.
            fprintf(fptr,"SECIM1\n");
            fclose(fptr);
            controlNktFiles ( nktFilesAmount,fileNames ); // dosya isimleri buradan cekiliyor.
            for(i=0; i<nktFilesAmount; i++)
            {
                FILE *fptr=fopen(outputFile,"a"); // output dosyasi onceden varsa icerigi siliniyor.
                locusAmount=0;

                char ConvertedBinaryFile[100]="" ;// asci ye cevrilen dosyalarin isimleri olacak .

                strcpy(menuFileNames[i],fileNames[i]);

                result1=validateFileHeaders(fileNames[i], outputFile, &rgb, &givenLocusAmount,&isBinary,menuFileNames[i]);

                if(isBinary && strstr(fileNames[i],"converted")==NULL)
                {

                    strcpy(ConvertedBinaryFile,"convertedTo");
                    strcat(ConvertedBinaryFile,fileNames[i]);
                    convertBinary(fileNames[i],ConvertedBinaryFile,rgb);//donusturme islemmi
                    strcpy(fileNames[i],ConvertedBinaryFile);


                }
                arr_rgb[i]= rgb;
                result2=0;

                isOk[i]=1; // her dosya hatasiz olarak tanimlanir hata varsa atanacaktir.
                int rowAmount =rowCounter(fileNames[i]);

                if(result1)

                {
                    result2=validateLocus(fileNames[i],outputFile,rowAmount, rgb, &locusAmount);

                }

                if(result1 && result2 && rowAmount==locusAmount) // satir sayisi ve sayilan nokta sayisi esitse gir.
                {


                    if(givenLocusAmount==locusAmount)
                    {
                        fprintf(fptr,"Dosyada hata yoktur.\n");

                    }
                    else if(locusAmount>givenLocusAmount)
                    {
                        fprintf(fptr,"Nokta sayisi baslik kisminda girilenden fazladir.\n");
                        isOk[i]=0;

                    }
                    else if(locusAmount<givenLocusAmount)
                    {
                        fprintf(fptr,"Nokta sayisi baslik kisminda verilenden azdir.\n");
                        isOk[i]=0; // nokta validasyonu saglanmazsa 0 yapacak.
                    }

                }
                else
                {
                    if(result1==1 && result1==1)// noktalar arasi bosluk varsa hata bastirmak icin.
                        fprintf(fptr,"Nokta sayisi hatalidir.\n");
                    isOk[i]=0;

                }
                arr_locusAmount[i]=givenLocusAmount; // hata yoksa baslik kismindan fonksiyonlara verilmek uzere bir diziye nokta sayisi degeri atanir.
                fclose(fptr);

            }
            int result=1; // eger hata varsa dosyadan okuma yapilacak.
            for(i=0; i<nktFilesAmount; i++)  // hata varsa ekrana bastirma.
            {

                if(isOk[i]==0) // herhangi bir bir doyada hata olmasi durumunda
                {
                    result=0;

                }
            }

            if(result==0)
            {

                FILE *fptr=fopen(outputFile,"r");
                if(fptr==NULL)
                {
                    printf("%s dosyasinda sorun bulunmaktadir!!!Dosya acilamadi.",fileNames[i]);
                    exit(1);
                }
                char ch='-';
                printf("\n");
                while(!feof(fptr))
                {
                    ch=fgetc(fptr);
                    printf("%c",ch);
                }
                isError=1;
            }

            if(!isError)
            {
                printf("Tum dosyalar uyumludur.\n");
            }
            fclose(fptr);
            flag=1;// birdeki islemlerin gerceklestigini belirtiyoruz.
            printf("\nValidasyon islemi tamamlanmistir.Devam etmek icin herhangi bir tusa basiniz.\n");
            getch();
            break;
        }

        case 2:// ister 2

        {
            if(!flag)
            {
                printf("Once bire  basip validasyon yapmaniz gerekmektedir. ");

            }
            else
            {
                fptr=fopen(outputFile,"a");
                fprintf(fptr,"\nSECIM 2\n");
                fclose(fptr);
                for(i=0; i<nktFilesAmount; i++)
                {


                    if(isOk[i]==0)
                    {
                        printf("%s dosyasinda sorun bulunmaktadir!!!Dosya acilamadi.\n",menuFileNames[i]);
                    }
                    else

                    {
                        printf("%s daki noktalarin birbirine olan uzakliklari hesaplaniyor...\n",menuFileNames[i]);
                        compareLength(fileNames[i],outputFile, arr_locusAmount[i], arr_rgb[i],menuFileNames[i]);
                    }
                }
                printf("\nIslem bitmistir.Devam etmek icin herhangi bir tusa basiniz.\n");
                fclose(fptr);

            }


            getch();
            break;
        }
        case 3://ister 3
        {
            if(!flag)
            {
                printf("Once bire  basip validasyon yapmaniz gerekmektedir. ");

            }
            else
            {
                fptr=fopen(outputFile,"a"); // dosya ekleme modunda aciliyor.
                fprintf(fptr,"\nSECIM3\n");
                fclose(fptr);
                for(i=0; i<nktFilesAmount; i++)
                {
                    if(isOk[i]==0)
                    {
                        printf("%s dosyasinda sorun bulunmaktadir!!!Dosya acilamadi.\n",menuFileNames[i]);
                    }
                    else
                    {
                        printf("%s deki noktalari icine alacak en kucuk kup  hesaplaniyor... \n",menuFileNames[i]);
                        cube(fileNames[i],outputFile,arr_locusAmount[i],arr_rgb[i],menuFileNames[i]);
                    }
                }
                fclose(fptr);
                printf("\nIslem bitmistir.Devam etmek icin herhangi bir tusa basiniz.\n");
            }


            getch();
            break;
        }
        case 4:// ister 4
        {
            if(!flag)
            {
                printf("Once bire  basip validasyon yapmaniz gerekmektedir. ");

            }
            else
            {
                fptr=fopen(outputFile,"a"); // dosya ekleme modunda aciliyor.
                fprintf(fptr,"\nSECIM4\n");
                fclose(fptr);

                char x1[10], y1[10], z1[10], r1[10];
                printf("x : ");
                scanf("%s",x1);
                printf("y : ");
                scanf("%s",y1);
                printf("z : ");
                scanf("%s",z1);
                printf("r : ");
                scanf("%s",r1);

                while(1)
                {
                    if(controlInputForNum(x1)==0 || controlInputForNum(y1)==0 || controlInputForNum(z1)==0 || controlInputForNum(r1)==0)
                    {

                        printf("x, y, z ve r degerlerini dogru formatta giriniz!!!\n");
                        printf("x : ");
                        scanf("%s",x1);
                        printf("y : ");
                        scanf("%s",y1);
                        printf("z : ");
                        scanf("%s",z1);
                        printf("r : ");
                        scanf("%s",r1);
                    }
                    else
                        break;
                }
                float xValue=atof(x1);
                float yValue=atof(y1);
                float zValue=atof(z1);
                float rValue=atof(r1);

                for(i=0; i<nktFilesAmount; i++)
                {


                    if(isOk[i]==0)
                    {
                        printf("%s dosyasinda sorun bulunmaktadir!!!Dosya acilamadi.",menuFileNames[i]);
                    }
                    else

                    {
                        printf("\n%s cember hesaplaniyor... \n",menuFileNames[i]);
                        sphereInfo(fileNames[i], outputFile, arr_locusAmount[i], arr_rgb[i], xValue, yValue, zValue,rValue,menuFileNames[i]);
                    }

                    fclose(fptr);
                }
                printf("\nIslem bitmistir.Devam etmek icin herhangi bir tusa basiniz.\n");
            }
            getch();
            break;
        }
        case 5: // Ister 5
        {
            if(!flag)
            {
                printf("Once bire  basip validasyon yapmaniz gerekmektedir. ");

            }
            else
            {
                fptr=fopen(outputFile,"a"); // dosya ekleme modunda aciliyor.
                fprintf(fptr,"\nSECIM5\n");
                fclose(fptr);
                for(i=0; i<nktFilesAmount; i++)
                {

                    if(isOk[i]==0)
                    {
                        printf("%s dosyasinda sorun bulunmaktadir!!!Dosya acilamadi.",menuFileNames[i]);
                    }
                    else
                    {
                        printf("\n%s nin noktalar arasi uzakliklarin ortalamalari hesaplaniyor ... \n",menuFileNames[i]);
                        distanceAverange(fileNames[i],outputFile,arr_locusAmount[i],arr_rgb[i],menuFileNames[i]);
                    }

                    fclose(fptr);
                }
                printf("\nIslem bitmistir.Devam etmek icin herhangi bir tusa basiniz.\n");
            }

            getch();
            break;
        }

        case 6: // Programdan cikis
        {
            for(i=0; i<nktFilesAmount; i++) // geçici dosyalar siliniyor.
            {
                if(strstr(fileNames[i],"convertedTo"))
                    remove(fileNames[i]);

            }
            return 1; // basariyla cikis yapilmistir.
            break;
        }

        };
    }
    return 0;
}
// .nkt uzantili dosyalarin sayisini dondurur.

int nktFileAmountCounter()
{

    DIR *folder; // directory pointeri tanimladik.
    struct dirent *dataEntry; // file bilgileri icin struct tanimladik.
    int counter=0;

    folder=opendir(".");
    if(folder==NULL)
    {
        printf("Dizin acilamadi!!");
    }

    while ( (dataEntry=readdir(folder)))
    {
        if(strstr(dataEntry->d_name,".nkt") && strstr(dataEntry->d_name,"output")==NULL&&strstr(dataEntry->d_name,"convertedTo")==NULL)
        {
            counter++;
        }

    }

    closedir(folder);
    return counter;
}
// .nkt uzantili dosyalarin isimlerini iki boyutlu diziye atar.
void controlNktFiles (int size,char nktFilePath[size][100] )
{
    DIR *folder;
    struct dirent *dataEntry;
    int i=0;
    folder=opendir(".");
    if(folder==NULL)
    {
        printf("Dizin acilamadi!!");
    }

    /*.nkt uzantili dosyalarin isimlerini nktFileParth adli bir char dizisine kopyaladik. */

    while ( (dataEntry=readdir(folder)))
    {
        if(strstr(dataEntry->d_name,".nkt")&&strstr(dataEntry->d_name,"output")==NULL&&strstr(dataEntry->d_name,"convertedTo")==NULL) // output ismi gecmiyorsa .
        {
            strcpy(nktFilePath[i],dataEntry->d_name);
            i++;
        }
    }

    closedir(folder);

}

int rowCounter (char *string_path)
{

    FILE *file=fopen(string_path,"r");
    char c=fgetc(file);
    int result=0;// dosya bitiş yerini EOF lı \n olarak algılamadıgından counter ımız 1 den baslattık.
    char temp;
    /* temp degiskenine okunan karakter bir oncesi rakam ise demek ki nokta var demektir.Yanii dosya sonunda bosluk yoksa da sorun olmayacak.*/
    while(1)
    {
        if(c=='\n')
        {
            result++;

        }

        if(c==EOF && isdigit(temp) ) // en son satirda noktalardan sonra bosluk yoksa satir sayisini bir artirir.
        {
            result++;
            break;
        }
        else if (c==EOF && isdigit(temp)==0)
            break;
        temp=c;
        c=fgetc(file);
    }
    fclose(file);
    return result-5;

}
// menu icin alinan string in istedigimiz degerlerde olup olmadigini kontrol ediyoruz.
int controlInputForNum(char *str)
{
    int isValid=1, flag=1;
    for(int i=0; i<strlen(str); i++)
    {
        if(!isdigit(str[i]))
        {
            if(i==0 && str[i]=='.')
            {
                isValid=0;
                break;
            }
            if(str[i]!='.' )
            {
                isValid=0;
                break;
            }
            else if(str[i]=='.' && !flag)
            {
                isValid=0;
                break;
            }
            else if(str[i]=='.')
            {
                flag=0;
            }
        }
    }
    return isValid;
}
// binary dosyamizi asciiye ceviriyoruz.
void convertBinary(char *filePath,char *outputPath,int rgb)
{

    int i,j,headerCnt=0;
    FILE *f;
    FILE *fout;

    f=fopen(filePath,"r");
    if(f==NULL)
    {

        printf("%s dosyasi acilamadi!!!\n",filePath);
        exit(1);
    }

    char rows[5][50];
    char ch='-';
    int result=1;
    for(i=0; i<5; i++)
    {
        ch='-';// alttaki donguye girebilmesi icin baslangic degeri atadik.

        for(j=0; ch!= '\n'; j++)
        {
            ch=fgetc(f);
            rows[i][j]=ch;
            headerCnt++; // fseek ile baslik kismini gececegiz.
        }
        rows[i][j]='\0';

    }

    fclose(f); // basliktaki karakter miktari alindiktan sonra dosya kapatiliyor.
    // ******************** Baslik kismi yazdirilacak.*********************

    fout=fopen(outputPath,"w"); // ascii formatili dosya aciliyor.
    for(i=0; i<5; i++)
    {
        fprintf(fout,"%s",rows[i]);
    }
    fclose(fout);
    //************binary dosyalar asciiye cevrilip yazdirilacak**************
    f=fopen(filePath,"rb");
    fout=fopen(outputPath,"a");
    if(f==NULL)
        exit(1);
    if(fout==NULL)
    {
        printf("Asciye cevrilen dosya olusmamis\n");
        exit(1);
    }

    fseek(f,headerCnt,0);//baslik kismini gectik.
    if(rgb)
    {
        struct locusRgb locus_rgb;
        while(1)
        {
            result =fread(&locus_rgb,sizeof(struct locusRgb),1,f);
            if(result!=1) // fread dosya sonuna kadar okuma yapmamissa.
                break;
            fprintf(fout,"%f %f %f %d %d %d\n",locus_rgb.x,locus_rgb.y,locus_rgb.z,locus_rgb.r,locus_rgb.g,locus_rgb.b);

        }
    }
    else
    {
        struct locusNonRgb nonRgb;
        while(1)
        {
            result= fread(&nonRgb,sizeof(struct locusNonRgb),1,f);
            if(result!=1)
                break;
            fprintf(fout,"%f %f %f\n",nonRgb.x,nonRgb.y,nonRgb.z);
        }

    }
    fclose(f);
    fclose(fout);
}
int validateFileHeaders(char *inputFilePath,char *outputFilePath,int *rgb,int *givenLocusAmount,int *isBinary,char *fileName)
{
    char rows[5][50];
    int flag =1;
    FILE *fptr = fopen(inputFilePath,"r");
    if(fptr==NULL)
    {
        printf("Dosya acilamadi!!!Dosyayi kontrol ediniz.");
        exit(1);
    }

    int i,j;
    char ch;

    for(i=0; i<5; i++)
    {
        ch='-';// alttaki donguye girebilmesi icin baslangic degeri atadik.

        for(j=0; ch!= '\n'; j++)
        {
            ch=fgetc(fptr);
            rows[i][j]=ch;
        }
        rows[i][j]='\0';
    }


    FILE *fptr2 =fopen(outputFilePath,"a"); // output dosyasina hatalar basilacak.
    // kontrol ifadeleri basliyor.
    // yorum satiri kontrolu
    fprintf(fptr2,"\n%s Dosyasi:\n",fileName);

    if(strstr(rows[0],"#")==NULL)
    {
        fprintf(fptr2,"Baslik bilgilerinde yorum satiri eksik.\n");
        flag=0;
    }
    // verison kontrol
    if(strstr(rows[1],"VERSION")==NULL)
    {

        fprintf(fptr2,"Baslik bolumunde versiyon bilgisi eksik.\n");
        flag=0;
    }
    else
    {
        int version = -1;
        char temp[20];
        sscanf(rows[1],"%s%d",temp,&version);// version bilgisini cektik.
        if(version!=1 )
        {
            fprintf(fptr2,"Version degeri yanlistir.\n");
            flag=0;
        }
    }

    // xyzrgb kontol
    if(strcmp(rows[2],"ALANLAR x y z\n") ==0)
    {
        *rgb =0;
    }
    else if( strcmp(rows[2],"ALANLAR x y z r g b\n")==0)
    {
        *rgb=1;
    }
    else
    {
        fprintf(fptr2,"Alanlar bilgisi yanlis girilmistir.\n");
        flag=0;

    }
    // nokta kontrolu
    if(strstr(rows[3],"NOKTALAR")==NULL)
    {
        fprintf(fptr2,"Baslikta nokta bilgisi eksik.\n");
        flag=0;
    }
    else
    {

        float x=-1;
        char temp[20];
        sscanf(rows[3],"%s%f",temp,&x);
        x = (x==(int)x) ? x :-1; // noktanin float olup olmadigini kontrol ettik.
        *givenLocusAmount=(int)x;
        if(x<0)
        {
            fprintf(fptr2,"Nokta verisi yanlis girilmistir.\n");
            flag=0;
        }
    }
    //data kontrolu
    if(strstr(rows[4],"DATA")==NULL)
    {
        fprintf(fptr2,"Baslikta data bilgisi eksik.\n");
        flag=0;
    }
    else
    {
        if(strstr(rows[4],"ascii")==NULL && strstr(rows[4],"binary")==NULL)
        {
            fprintf(fptr2,"Data binary ve ascii disinda bir veri turu olamaz.\n");
            flag=0;
        }
    }
    if(strstr(rows[4],"binary"))
    {
        *isBinary=1;
    }
    else
    {
        *isBinary=0;
    }


    fclose(fptr);
    fclose(fptr2);
    return flag;
}
int validateLocus(char *fileName,char *outputFile, int givenLocusAmount,int rgb,int *locusAmount)   //locusType : yani g,r,b bilgileri ile mi okunacak..
{
    FILE *fptr = fopen(fileName,"r");
    if(fptr==NULL)
    {
        printf("Dosya acilamadi!!!Dosyayi kontrol ediniz.A");
        exit(1);
    }

    FILE *fptr2=fopen(outputFile,"a");//validasyon mesajlarÄ±nÄ± output.nkt dosyasÄ±na yazdÄ±rÄ±yoruz.
    if(fptr2==NULL)
    {
        printf("Dosya acilamadi!!!Dosyayi kontrol ediniz.B");
        exit(1);
    }

    int i,j;
    int flag=1;
    float x=-1, y=-1, z=-1;
    int g=-1, r=-1, b=-1;
    char rows[5][100];
    char str[200];
    char ch;
    for(i=0; i<5; i++) // baslik kismi geciliyor.
    {
        ch='-';// alttaki donguye girebilmesi icin baslangic degeri atadik.

        for(j=0; ch!= '\n'; j++)
        {
            ch=fgetc(fptr);
            rows[i][j]=ch;
        }
        rows[i][j]='\0';
    }


    // rgbli nokta kontrolu.

    if(rgb) //tum nokta satirlarini matrise atip sonradan sirayla noktalari kontrol edecegiz.
    {

        flag=1;
        for(i=0; i<givenLocusAmount; i++)
        {
            fgets(str,70,fptr); // fgets ile satir satir dosyadan okuma yapiyoruz.
            sscanf(str,"%f%f%f%d%d%d",&x,&y,&z,&r,&b,&g);
            if(x==-1 && y==-1 && z==-1 && g==-1 && r==-1 && b==-1)
            {
                flag=0;
                break;
            }

            else if(g==-1 || r==-1 || b==-1)
            {
                fprintf(fptr2,"%d. %s",i+1,"nokta verisi nokta veya r g b bilgileri hatali verilmistir.\n");
                flag=0;
                *locusAmount=*locusAmount+1;

                break;
            }
            else if(g>255 ||g<0 || r>255 ||r<0 || b>255 || b<0)
            {

                fprintf(fptr2,"%d. %s",i+1,"nokta verisinin rgb bilgileri 0-255 araliginnda degil.\n");
                *locusAmount=*locusAmount+1;
            }
            else
            {
                *locusAmount=*locusAmount+1;
            }


            x=-1, y=-1, z=-1;
            g=-1, r=-1, b=-1;

        }

    }
    // rgbsiz dosya kontrolu.
    else
    {
        flag=1;


        for(i=0; i<givenLocusAmount; i++)
        {
            fgets(str,70,fptr);
            sscanf(str,"%f%f%f%d%d%d",&x,&y,&z,&r,&b,&g);

            if(x==-1 && y==-1 && z==-1) // nokta yoksa .
            {
                flag =0;
                break;
            }
            else if(x==-1 || y==-1 || z==-1)
            {
                fprintf(fptr2,"%d. %s\n",i+1,"nokta verileri eksik girilmistir.");

                flag=0;
                *locusAmount=*locusAmount+1;
                break;// hata bulduginda cik.
            }
            else
            {
                *locusAmount=*locusAmount+1;
            }

            x=-1, y=-1, z=-1;
        }

    }


    fclose(fptr);
    fclose(fptr2);

    if(flag==1)
    {
        return 1;
    }
    else
        return 0;
}

void compareLength(char *filePath,char *outputFilePath,int locusAmount,int rgb,char *fileName)
{
    FILE *fptr = fopen(filePath,"r");// okunacak dosya aciliyor.
    if(fptr==NULL)
    {
        printf("Dosya acilamadi.1");
        exit(1);
    }
    FILE *outptr=fopen(outputFilePath,"a"); // output dosyasi append modunda aciliyor.

    if(fptr==NULL)
    {
        printf("%s dosyasi acilamadi2.",filePath);
        exit(1);
    }
    char ch;
    char rows[5][50];
    float x[locusAmount],y[locusAmount],z[locusAmount];
    int r[locusAmount],g[locusAmount],b[locusAmount];
    int furtIndex_1,furtIndex_2;
    int nearIndex_1,nearIndex_2;// en yakin ve en uzak noktalari bulmak icin index  tanimladik.
    int i,j; // dongu icin sayac
    double nearstDistance,futhestDistance,distance ; // noktalar arasindaki mesafe degiskeni


    for(i=0; i<5; i++)
    {
        ch='-';// alttaki donguye girebilmesi icin baslangic degeri atadik.

        for(j=0; ch!= '\n'; j++)
        {
            ch=fgetc(fptr);
            rows[i][j]=ch;
        }
        rows[i][j]='\0';
    }


    if(rgb) // rgbli hesaplama
    {
        for(i=0; i<locusAmount; i++)
            fscanf(fptr,"%f%f%f%d%d%d",&x[i],&y[i],&z[i],&r[i],&g[i],&b[i]);


    }
    // rgb olmadan.
    else
    {
        for(i=0; i<locusAmount; i++)
            fscanf(fptr,"%f%f%f",&x[i],&y[i],&z[i]);

    }

    nearstDistance = sqrt(pow(x[0]-x[1],2)+pow(y[0]-y[1],2)+pow(z[0]-z[1],2));
    futhestDistance=nearstDistance;

    //hesaplama
    for(i=0; i<locusAmount-1; i++)
    {
        for(j=i+1; j<locusAmount; j++) // Ister 2ve 5
        {
            distance =sqrt(pow(x[i]-x[j],2)+pow(y[i]-y[j],2)+pow(z[i]-z[j],2));

            if(nearstDistance>distance)
            {
                nearstDistance=distance;
                nearIndex_1 =i;
                nearIndex_2 =j;
            }
            if(futhestDistance<distance)
            {
                futhestDistance=distance;
                furtIndex_1=i;
                furtIndex_2=j;
            }

        }
    }
    fprintf(outptr,"\n%s Dosyasi :\n",fileName);

    if(rgb)
    {

        fprintf(outptr,"%f %f %f %d %d %d - %d.Nokta\n",x[nearIndex_1],y[nearIndex_1],z[nearIndex_1],r[nearIndex_1],g[nearIndex_1],b[nearIndex_1],nearIndex_1+1);
        fprintf(outptr,"%f %f %f %d %d %d - %d.Nokta\n",x[nearIndex_2],y[nearIndex_2],z[nearIndex_2],r[nearIndex_2],g[nearIndex_2],b[nearIndex_2],nearIndex_2+1);
        fprintf(outptr,"%f %f %f %d %d %d - %d.Nokta\n",x[furtIndex_1],y[furtIndex_1],z[furtIndex_1],r[furtIndex_1],b[furtIndex_1],b[furtIndex_1],furtIndex_1+1);
        fprintf(outptr,"%f %f %f %d %d %d - %d.Nokta\n",x[furtIndex_2],y[furtIndex_2],z[furtIndex_2],r[furtIndex_2],b[furtIndex_2],b[furtIndex_2],furtIndex_2+1);


    }
    else
    {

        fprintf(outptr,"%f %f %f - %d.Nokta\n",x[nearIndex_1],y[nearIndex_1],z[nearIndex_1],nearIndex_1+1);
        fprintf(outptr,"%f %f %f - %d.Nokta\n",x[nearIndex_2],y[nearIndex_2],z[nearIndex_2],nearIndex_2+1);
        fprintf(outptr,"%f %f %f - %d.Nokta\n",x[furtIndex_1],y[furtIndex_1],z[furtIndex_1],furtIndex_1+1);
        fprintf(outptr,"%f %f %f - %d.Nokta\n",x[furtIndex_2],y[furtIndex_2],z[furtIndex_2],furtIndex_2+1);

    }


    fclose(outptr);
    fclose(fptr);
}
void cube(char *filePath,char *outputFilePath,int locusAmount,int rgb,char *fileName)
{
    FILE *fptr = fopen(filePath,"r");// okunacak dosya aciliyor.
    if(fptr==NULL)
    {
        printf("Dosya acilamadi");
        exit(1);
    }
    FILE *outptr=fopen(outputFilePath,"a"); // output dosyasi append modunda aciliyor.

    if(fptr==NULL)
    {
        printf("%s dosyasi acilamadi.",filePath);
        exit(1);
    }
    char ch;
    char rows[5][50];
    float x[locusAmount],y[locusAmount],z[locusAmount];
    float max[3]= {-1,-1,-1};
    float min[3]= {9999,9999,9999};
    int r[locusAmount],g[locusAmount],b[locusAmount];
    int i,j; // dongu icin sayac


    for(i=0; i<5; i++)
    {
        ch='-';// alttaki donguye girebilmesi icin baslangic degeri atadik.

        for(j=0; ch!= '\n'; j++)
        {
            ch=fgetc(fptr);
            rows[i][j]=ch;
        }
        rows[i][j]='\0';
    }


    if(rgb) // rgbli hesaplama
    {
        for(i=0; i<locusAmount; i++)
            fscanf(fptr,"%f%f%f%d%d%d",&x[i],&y[i],&z[i],&r[i],&g[i],&b[i]);


    }
    // rgb olmadan.
    else
    {
        for(i=0; i<locusAmount; i++)
            fscanf(fptr,"%f%f%f",&x[i],&y[i],&z[i]);

    }

    for(i=0; i<locusAmount; i++)
    {
        if(x[i]>max[0]) // Ister -3
        {
            max[0]=x[i];
        }
        else if(x[i]<min[0])
        {
            min[0]=x[i];
        }
        if(y[i]>max[1])
        {
            max[1]=y[i];

        }
        else if(y[i]<min[1])
        {
            min[1]=y[i];
        }
        if(z[i]>max[2])
        {
            max[2]=z[i];
        }
        else if(z[i]<min[2])
        {
            min[2]=z[i];
        }
    }

    float dif=max[0]-min[0]; // aralarindaki farki buluyoruz.

    outptr=fopen(outputFilePath,"a");
    fprintf(outptr,"\n%s Dosyasi :\n",fileName);
    fprintf(outptr,"%f %f %f\n",min[0],min[1],min[2]);
    fprintf(outptr,"%f %f %f\n",min[0]+dif,min[1],min[2]);
    fprintf(outptr,"%f %f %f\n",min[0],min[1]+dif,min[2]);
    fprintf(outptr,"%f %f %f\n",min[0],min[1],min[2]+dif);
    fprintf(outptr,"%f %f %f\n",min[0]+dif,min[1]+dif,min[2]);
    fprintf(outptr,"%f %f %f\n",min[0]+dif,min[1],min[2]+dif);
    fprintf(outptr,"%f %f %f\n",min[0],min[1]+dif,min[2]+dif);
    fprintf(outptr,"%f %f %f\n",min[0]+dif,min[1]+dif,min[2]+dif);
    fclose(fptr);
    fclose(outptr);
}

void sphereInfo(char *filePath, char *outputFile, int locusAmount, int rgb, float x1, float y1, float z1, float r1,char *fileName) // secim 4
{
    FILE *fptr=fopen(filePath,"r");
    if(fptr==NULL)
    {
        printf("%s dosyasi acilamadi!!!Dosyayi kontrol ediniz.",filePath);
        exit(1);
    }

    FILE *outptr=fopen(outputFile,"a");
    if(outptr==NULL)
    {
        printf("Dosya acilamadi!!!Dosyayi kontrol ediniz.");
        exit(1);
    }
    int i, j; // dongu icin sayac
    int cnt ;// kac nokta cemberin icinde kalacak onun hesaplamasini yapacagiz.
    float x[locusAmount],y[locusAmount],z[locusAmount];
    int r[locusAmount],g[locusAmount],b[locusAmount];
    char ch;
    char rows[5][50];

    for(i=0; i<5; i++) // baslik kismi geciliyor.
    {
        ch='-';// alttaki donguye girebilmesi icin baslangic degeri atadik.

        for(j=0; ch!= '\n'; j++)
        {
            ch=fgetc(fptr);
            rows[i][j]=ch;
        }
        rows[i][j]='\0';
    }
    fprintf(outptr,"\n%s Dosyasi :\n",fileName);
    fprintf(outptr,"%f %s\n",x1,"=(Kullanicidan alinan kurenin x degeri)");
    fprintf(outptr,"%f %s\n",y1,"=(Kullanicidan alinan kurenin y degeri)");
    fprintf(outptr,"%f %s\n",z1,"=(Kullanicidan alinan kurenin z degeri)");
    fprintf(outptr,"%f %s\n",r1,"=(Kullanicidan alinan kurenin r degeri)");
    fprintf(outptr,"%s",rows[2]);
    fprintf(outptr,"%s",rows[3]);
    fprintf(outptr,"%s",rows[4]);

    if(rgb)
    {
        cnt=0;
        for(i=0; i<locusAmount; i++)
        {
            fscanf(fptr,"%f%f%f%d%d%d",&x[i],&y[i],&z[i],&g[i],&r[i],&b[i]);
            double length=sqrt(pow(x1-x[i],2)+pow(y1-y[i],2)+pow(z1-z[i],2));
            if(length<r1)
            {
                fprintf(outptr,"%f %f %f %d %d %d%s%d\n",x[i],y[i],z[i],g[i],r[i],b[i],"        Nokta numarasi : ",i+1);
                cnt++;
            }
        }
        if(cnt==0)
        {
            fprintf(outptr,"Cemberin icinde kalan nokta yoktur.\n");
        }


    }
    else
    {
        cnt=0;
        for(i=0; i<locusAmount; i++)
        {
            fscanf(fptr,"%f%f%f",&x[i],&y[i],&z[i]);
            double length=sqrt(pow(x1-x[i],2)+pow(y1-y[i],2)+pow(z1-z[i],2));
            if(length<r1)
            {
                fprintf(outptr,"%f %f %f%s%d\n",x[i],y[i],z[i],"        Nokta numarasi : ",i+1);
                cnt++;
            }

        }
        if(cnt==0)
        {
            fprintf(outptr,"Cemberin icinde kalan nokta yoktur.\n");
        }

    }

    fclose(fptr);
    fclose(outptr);
}

void distanceAverange(char *filePath,char *outputFilePath,int locusAmount,int rgb,char *fileName)
{

    FILE *fptr = fopen(filePath,"r");// okunacak dosya aciliyor.
    if(fptr==NULL)
    {
        printf("Dosya acilamadi.1");
        exit(1);
    }
    FILE *outptr=fopen(outputFilePath,"a"); // output dosyasi append modunda aciliyor.

    if(fptr==NULL)
    {
        printf("%s dosyasi acilamadi2.",filePath);
        exit(1);
    }
    char ch;
    char rows[5][50];
    float x[locusAmount],y[locusAmount],z[locusAmount];
    int r[locusAmount],g[locusAmount],b[locusAmount];
    int i,j; // dongu icin sayac
    double distance=0 ; // noktalar arasindaki mesafe degiskeni
    double distanceSum=0;

    for(i=0; i<5; i++)
    {
        ch='-';// alttaki donguye girebilmesi icin baslangic degeri atadik.

        for(j=0; ch!= '\n'; j++)
        {
            ch=fgetc(fptr);
            rows[i][j]=ch;
        }
        rows[i][j]='\0';
    }


    if(rgb) // rgbli hesaplama
    {
        for(i=0; i<locusAmount; i++)
            fscanf(fptr,"%f%f%f%d%d%d",&x[i],&y[i],&z[i],&r[i],&g[i],&b[i]);


    }
    // rgb olmadan.
    else
    {
        for(i=0; i<locusAmount; i++)
            fscanf(fptr,"%f%f%f",&x[i],&y[i],&z[i]);

    }

    //hesaplama
    double counter=0;
    for(i=0; i<locusAmount-1; i++)
    {
        for(j=i+1; j<locusAmount; j++) // Ister 5
        {
            distance =sqrt(pow(x[i]-x[j],2)+pow(y[i]-y[j],2)+pow(z[i]-z[j],2));

            distanceSum+=distance; //mesafeleri topluyoruz.
            counter++;
        }
    }
    double distanceAverange = distanceSum / counter;

    outptr=fopen(outputFilePath,"a");
    fprintf(outptr,"\n%s Dosyasi :\n",fileName);
    fprintf(outptr,"%lf\n",distanceAverange);
    fclose(fptr);
    fclose(outptr);


}
