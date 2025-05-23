/*
BT lon - thuc hanh
Pham Ngoc Tu - 6551071092

Mo ta: Chuong trinh quan ly benh nhan tam than
Cac ly thuyet da ung dung: Ham, con tro, cap phat dong, xu ly tep, kieu cau truc, danh sach lien ket
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

#define ID_SIZE 10
#define DESCR 200
#define MIN_AGE 3
#define MAX_AGE 150
#define PHONE_SIZE 15

//thong tin ca nhan (px = Px)
typedef struct pxIn4{
    char pxID[ID_SIZE];
    char name[50];
    int age;
    char gender[10];
    char address[100];
    char contactNum[PHONE_SIZE];
}pxIn4; 

//chan doan benh (diagnosis = dx)
typedef struct pxDx{
    char dxID[ID_SIZE];
    char dx[DESCR]; //chan doan benh
    char sx[DESCR]; //trieu chung benh
    char dxDate[12]; //ngay chuan doan
}pxDx;

//dieu tri va thuoc (treatment = trt)
typedef struct trt{
    char trtID[ID_SIZE];
    char trtPlan[DESCR]; //Phac do dieu tri
    char meds[DESCR]; //danh sach thuoc dang sd
    float dosage; //lieu luong thuoc
    char trtStartDate[12]; //ngay bat dau dieu tri
    char trtEndDate[12]; //ngay ket thuc dieu tri
}trt;

typedef struct Node{
    pxIn4* px;
    pxDx* dx;
    trt* trt;
    struct Node* prep;
    struct Node* next; 
}Node;

//Khai bao ham
void Menu();
void inputPxIn4(pxIn4* px);
void inputPxDx(pxDx* dx);
void inputTrt(trt* t);
void clearBuffer();

bool isName(char* name);
bool isDate(char* date);
bool isPhoneNum(char* phone);
bool isID(char* id);
pxIn4* makePxIn4();
pxDx* makePxDx();
trt* makeTrt();
Node* makeNode(pxIn4* px, pxDx* dx, trt* trt);

void displayAllPx(Node* head);
void searchPx(Node* head);
void updatePx(Node* head);
void deletePx(Node** head);
int compareIDs(const char* id1, const char* id2);
void insertSorted(Node** head, Node* newNode);
void freeMemory(Node** head);
void displayPxInfo(Node* node);

//File 
void saveFile(Node* head, const char* fileName);
void loadFile(Node** head, const char* fileName);
void fileMenu(Node** head);

int main(){
    Node* head = NULL;
    int choice;
    
    while(1){
        Menu();
        printf("Nhap lua chon cua ban: ");
        if (scanf("%d", &choice) != 1){
            printf("Nhap sai dinh dang. Vui long nhap lai.\n");
            clearBuffer();
            continue;
        }
        clearBuffer();
        
        switch(choice){
            case 1:
                //Them benh nhan moi
            	{
                    pxIn4* px = makePxIn4();
                    pxDx* dx = makePxDx();
                    trt* t = makeTrt();
                    
                    inputPxIn4(px);
                    inputPxDx(dx);
                    inputTrt(t);
                    
                    Node* newNode = makeNode(px, dx, t);
                    
                    //Them node vao danh sach va sap xep theo ID 
                    insertSorted(&head, newNode);
                    
                    printf("Them benh nhan thanh cong!\n");
                }
                break;
            case 2:
                displayAllPx(head);
                break;
            case 3:
                searchPx(head);
                break;
            case 4:
                updatePx(head);
                break;
            case 5:
                deletePx(&head);
                break;
		    case 6:
		        fileMenu(&head);
		        break;
		    case 0:
		        printf("Ban co muon luu du lieu truoc khi thoat? (1: Co, 0: Khong): ");
		        int saveChoice;
		        scanf("%d", &saveChoice);
		        clearBuffer();
		        if (saveChoice == 1){
		            char fileName[100];
		            printf("Nhap ten file de luu: ");
		            fgets(fileName, 100, stdin);
		            fileName[strcspn(fileName, "\n")] = '\0';
		            saveFile(head, fileName);
		        }
		        printf("Thoat chuong trinh.\n");
		        freeMemory(&head); 
				exit(0);
            default:
                printf("Lua chon khong hop le. Vui long chon lai.\n");
        }
    }
}

void Menu(){
    printf("\n===== QUAN LY BENH NHAN =====\n");
    printf("1. Them benh nhan moi\n");
    printf("2. Hien thi danh sach benh nhan\n");
    printf("3. Tim kiem benh nhan\n");
    printf("4. Cap nhat thong tin benh nhan\n");
    printf("5. Xoa benh nhan\n");
    printf("6. Luu/Doc du lieu vao file\n");
    printf("0. Thoat\n");
}

//Don buffer
void clearBuffer(){
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

//Check name
bool isName(char* name){
    if (strlen(name) == 0) return false;
        
    for (int i = 0; *(name+i) != '\0'; i++){
        if (!isalpha(*(name+i)) && !isspace(*(name+i))) return false;
    }
    return true;
}

//check so dien thoai chi chua so
bool isPhoneNum(char* phone){
    if (strlen(phone) == 0) return false;
        
    for(int i = 0; *(phone+i) != '\0'; i++){
        if(!isdigit(*(phone+i))){
            return false;
        }
    }
    return true;
}

//check ID
bool isID(char* id){
    if (strlen(id) == 0) return false;
        
    for(int i = 0; *(id+i) != '\0'; i++){
        if(!isalnum(*(id+i))) return false;
    }
    return true;
}

//check dinh dang ngay
bool isDate(char* date){
    if(strlen(date) != 10) return false;
    
    //check format DD/MM/YYYY
    for(int i = 0; i < 10; i++){
        if(i == 2 || i == 5){
            if(*(date+i) != '/') return false;
        } else{
            if(!isdigit(*(date+i))) return false;
        }
    }
    
    //check ngay thang hop ly
    int day = (*(date+0) - '0') * 10 + (*(date+1) - '0');
    int month = (*(date+3) - '0') * 10 + (*(date+4) - '0');
    int year = (*(date+6) - '0') * 1000 + (*(date+7) - '0') * 100 + (*(date+8) - '0') * 10 + (*(date+9) - '0');
    
    if(day < 1 || month < 1 || month > 12 || year < 1900 || year > 2100) return false;
        
    //check so ngay trong thang
    int daysInMonth[] ={0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    
    //check nam nhuan
    if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
        daysInMonth[2] = 29;
        
    if (day > *(daysInMonth+month)) return false;
    
    return true;
}

//Cap phat bo nho cho cac struct
pxIn4* makePxIn4(){
    pxIn4* px = (pxIn4*)calloc(1, sizeof(pxIn4));
    if(!px){
        printf("Loi cap phat dong.\n"); exit(1);
    }
    return px;
}

pxDx* makePxDx(){
    pxDx* dx = (pxDx*)calloc(1, sizeof(pxDx));
    if(!dx){
        printf("Loi cap phat dong.\n"); exit(1);
    }
    return dx;
}

trt* makeTrt(){
    trt* t = (trt*)calloc(1, sizeof(trt));
    if(!t){
        printf("Loi cap phat dong.\n"); exit(1);
    }
    return t;
}

Node* makeNode(pxIn4* px, pxDx* dx, trt* trt){
    Node* node = (Node*)malloc(sizeof(Node));
    if(!node){
        printf("Loi cap phat dong cho node.\n"); exit(1);
    }
    node->px = px;
    node->dx = dx;
    node->trt = trt;
    node->prep = NULL;
    node->next = NULL;
    return node;
}

//So sanh ID 
int compareIDs(const char* id1, const char* id2){
    return strcmp(id1, id2);
}

//chen node vao danh sach theo thu tu ID
void insertSorted(Node** head, Node* newNode){
    //Neu danh sach rong hoac nut moi co ID nho hon nut dau tien
    if(*head == NULL || compareIDs(newNode->px->pxID, (*head)->px->pxID) < 0){
        newNode->next = *head;
        if (*head != NULL){
            (*head)->prep = newNode;
        }
        *head = newNode;
        return;
    }
    
    //Tim vi tri de chen node moi 
    Node* now = *head;
    while(now->next != NULL && compareIDs(newNode->px->pxID, now->next->px->pxID) >= 0){
        now = now->next;
    }
    
    //Chen nut moi vao sau node hien tai 
    newNode->next = now->next;
    if (now->next != NULL){
        now->next->prep = newNode;
    }
    now->next = newNode;
    newNode->prep = now;
}

//Giai phong bo nho 
void freeMemory(Node** head){
    Node* now = *head;
    Node* next;
    
    while (now != NULL){
        next = now->next;
        free(now->px);
        free(now->dx);
        free(now->trt);
        free(now);
        now = next;
    }
    
    *head = NULL;
}

//Nhap thong tin ca nhan benh nhan
void inputPxIn4(pxIn4* px){
    printf("\n===== NHAP THONG TIN CA NHAN BENH NHAN =====\n");
    
    //Nhap ID
    while(1){
        printf("Nhap ID benh nhan (toi da %d ky tu): ", ID_SIZE-1);
        char tmpID[ID_SIZE*2];
        fgets(tmpID, sizeof(tmpID), stdin);
        tmpID[strcspn(tmpID, "\n")] = '\0';
        
        if(strlen(tmpID) == 0){
            printf("ID benh nhan khong duoc de trong. Vui long nhap lai.\n");
            continue;
        }
        
        if(strlen(tmpID) >= ID_SIZE){
            printf("ID qua dai, chi duoc phep nhap toi da %d ki tu. Vui long nhap lai.\n", ID_SIZE-1);
            continue;
        }
        
        if(!isID(tmpID)){
            printf("ID chi duoc chua chu cai va so. Vui long nhap lai.\n");
            continue;
        }
        
        strcpy(px->pxID, tmpID);
        break;
    }
    
    //Nhap ten
    while(1){
        printf("Nhap ho ten benh nhan: ");
        char tmpName[100];
        fgets(tmpName, sizeof(tmpName), stdin);
        tmpName[strcspn(tmpName, "\n")] = '\0';
        
        if (strlen(tmpName) == 0){
            printf("Ten khong duoc de trong. Vui long nhap lai.\n");
            continue;
        }
        
        if(strlen(tmpName) >= sizeof(px->name)){
            printf("Ten qua dai. Vui long nhap lai.\n"); continue;
        }
        
        if(isName(tmpName)){
            strcpy(px->name, tmpName);
            break;
        }else{
            printf("Ten khong hop le. Vui long nhap lai.\n");
        }
    }
    
    //Nhap tuoi
    while(1){
        printf("Nhap tuoi (%d-%d): ", MIN_AGE, MAX_AGE);
        if(scanf("%d", &px->age) == 1){
            if (px->age >= MIN_AGE && px->age <= MAX_AGE){
                clearBuffer();
                break;
            } else{
                printf("Tuoi khong hop le. Vui long nhap lai (tu %d den %d).\n", MIN_AGE, MAX_AGE);
                clearBuffer();
            }
        }else{
            printf("Nhap sai dinh dang. Vui long nhap lai.\n");
            clearBuffer();
        }
    }
    
    //Nhap gioi tinh
    while(1){
        printf("Nhap gioi tinh (Nam/Nu): ");
        char tmpGender[20];
        fgets(tmpGender, sizeof(tmpGender), stdin);
        tmpGender[strcspn(tmpGender, "\n")] = '\0';
        
        if (strcmp(tmpGender, "Nam") == 0 || strcmp(tmpGender, "Nu") == 0){
            strcpy(px->gender, tmpGender);
            break;
        }else{
            printf("Gioi tinh khong hop le. Vui long nhap 'Nam' hoac 'Nu'.\n");
        }
    }
    
    //Nhap dia chi
    while(1){
        printf("Nhap dia chi: ");
        char tmpAddress[150];
        fgets(tmpAddress, sizeof(tmpAddress), stdin);
        tmpAddress[strcspn(tmpAddress, "\n")] = '\0';
        
        if(strlen(tmpAddress) > 0){
            if (strlen(tmpAddress) >= sizeof(px->address)){
                printf("Dia chi qua dai. Vui long nhap lai.\n");
                continue;
            }
            strcpy(px->address, tmpAddress);
            break;
        }else{
            printf("Dia chi khong duoc de trong. Vui long nhap lai.\n");
        }
    }
    
    //Nhap so dien thoai
    while(1){
        printf("Nhap so dien thoai (toi da %d so): ", PHONE_SIZE-1);
        char tmpPhone[PHONE_SIZE];
        fgets(tmpPhone, sizeof(tmpPhone), stdin);
        tmpPhone[strcspn(tmpPhone, "\n")] = '\0';
        
        if (strlen(tmpPhone) == 0){
            printf("So dien thoai khong duoc de trong. Vui long nhap lai.\n");
            continue;
        }
        
        if(!isPhoneNum(tmpPhone)){
            printf("So dien thoai chi duoc chua cac chu so. Vui long nhap lai.\n");
            continue;
        }
        
        if(strlen(tmpPhone) >= PHONE_SIZE){
            printf("So dien thoai qua dai, chi duoc phep nhap toi da %d so. Vui long nhap lai.\n", PHONE_SIZE-1);
            continue;
        }
        
        strcpy(px->contactNum, tmpPhone);
        break;
    }
    
    printf("Nhap thong tin ca nhan benh nhan thanh cong!\n");
}

//Nhap thong tin chan doan benh
void inputPxDx(pxDx* dx){
    printf("\n===== NHAP THONG TIN CHAN DOAN BENH =====\n");
    
    //Nhap ID chan doan
    while(1){
        printf("Nhap ID chan doan (toi da %d ky tu): ", ID_SIZE-1);
        char tmpID[ID_SIZE];
        fgets(tmpID, sizeof(tmpID), stdin);
        tmpID[strcspn(tmpID, "\n")] = '\0';
        
        if(strlen(tmpID) == 0){
            printf("ID chan doan khong duoc de trong. Vui long nhap lai.\n");
            continue;
        }
        
        if(!isID(tmpID)){
            printf("ID chi duoc chua chu cai va so. Vui long nhap lai.\n");
            continue;
        }
        
        if (strlen(tmpID) >= ID_SIZE){
            printf("ID qua dai, chi duoc phep nhap toi da %d ki tu. Vui long nhap lai.\n", ID_SIZE-1);
            continue;
        }
        
        strcpy(dx->dxID, tmpID);
        break;
    }
    
    //Nhap chan doan benh
    while(1){
        printf("Nhap chan doan benh:\n");
        fgets(dx->dx, DESCR, stdin);
        dx->dx[strcspn(dx->dx, "\n")] = '\0';
        
        if(strlen(dx->dx) > 0){
            break;
        } else{
            printf("Chan doan benh khong duoc de trong. Vui long nhap lai.\n");
        }
    }
    
    //Nhap trieu chung
    while(1){
        printf("Nhap trieu chung benh:\n");
        fgets(dx->sx, DESCR, stdin);
        dx->sx[strcspn(dx->sx, "\n")] = '\0';
        
        if(strlen(dx->sx) > 0){
            break;
        }else{
            printf("Trieu chung benh khong duoc de trong. Vui long nhap lai.\n");
        }
    }
    
    //Nhap ngay chan doan
    while(1){
        printf("Nhap ngay chan doan (DD/MM/YYYY): ");
        char tmpDate[20];
        fgets(tmpDate, sizeof(tmpDate), stdin);
        tmpDate[strcspn(tmpDate, "\n")] = '\0';
        
        if (isDate(tmpDate)){
            strcpy(dx->dxDate, tmpDate);
            break;
        } else{
            printf("Dinh dang ngay khong hop le. Vui long nhap lai.\n");
        }
    }
    
    printf("Nhap thong tin chan doan benh thanh cong!\n");
}

//Nhap thong tin dieu tri
void inputTrt(trt* t){
    printf("\n===== NHAP THONG TIN DIEU TRI =====\n");
    
    //Nhap ID dieu tri
    while(1){
        printf("Nhap ID dieu tri (toi da %d ky tu): ", ID_SIZE-1);
        char tmpID[ID_SIZE*2];
        fgets(tmpID, sizeof(tmpID), stdin);
        tmpID[strcspn(tmpID, "\n")] = '\0';
        
        if(strlen(tmpID) == 0){
            printf("ID dieu tri khong duoc de trong. Vui long nhap lai.\n");
            continue;
        }
        
        if(!isID(tmpID)){
            printf("ID chi duoc chua chu cai va so. Vui long nhap lai.\n");
            continue;
        }
        
        if(strlen(tmpID) >= ID_SIZE){
            printf("ID qua dai, chi duoc phep nhap toi da %d ki tu. Vui long nhap lai.\n", ID_SIZE-1);
            continue;
        }
        
        strcpy(t->trtID, tmpID);
        break;
    }
    
    //Nhap phac do dieu tri
    while(1){
        printf("Nhap phac do dieu tri:\n");
        fgets(t->trtPlan, DESCR, stdin);
        t->trtPlan[strcspn(t->trtPlan, "\n")] = '\0';
        
        if(strlen(t->trtPlan) > 0){
            break;
        } else{
            printf("Phac do dieu tri khong duoc de trong. Vui long nhap lai.\n");
        }
    }
    
    //Nhap danh sach thuoc
    while(1){
        printf("Nhap danh sach thuoc dang su dung:\n");
        fgets(t->meds, DESCR, stdin);
        t->meds[strcspn(t->meds, "\n")] = '\0';
        
        if(strlen(t->meds) > 0){
            break;
        } else{
            printf("Danh sach thuoc khong duoc de trong. Vui long nhap lai.\n");
        }
    }
    
    //Nhap lieu luong thuoc
    while(1){
        printf("Nhap lieu luong thuoc: ");
        if(scanf("%f", &t->dosage) == 1){
            if (t->dosage > 0){
                clearBuffer();
                break;
            }else{
                printf("Lieu luong thuoc phai lon hon 0. Vui long nhap lai.\n");
                clearBuffer();
            }
        }else{
            printf("Nhap sai dinh dang. Vui long nhap lai.\n");
            clearBuffer();
        }
    }
    
    //Nhap ngay bat dau dieu tri
    while(1){
        printf("Nhap ngay bat dau dieu tri (DD/MM/YYYY): ");
        char tmpDate[20];
        fgets(tmpDate, sizeof(tmpDate), stdin);
        tmpDate[strcspn(tmpDate, "\n")] = '\0';
        
        if (isDate(tmpDate)){
            strcpy(t->trtStartDate, tmpDate);
            break;
        }else{
            printf("Dinh dang ngay khong hop le. Vui long nhap lai.\n");
        }
    }
    
    //Nhap ngay ket thuc dieu tri
    while(1){
        printf("Nhap ngay ket thuc dieu tri (DD/MM/YYYY, hoac de trong neu chua xac dinh): ");
        char tmpDate[20];
        fgets(tmpDate, sizeof(tmpDate), stdin);
        tmpDate[strcspn(tmpDate, "\n")] = '\0';
        
        if (strlen(tmpDate) == 0 || isDate(tmpDate)){
            strcpy(t->trtEndDate, tmpDate);
            break;
        } else{
            printf("Dinh dang ngay khong hop le. Vui long nhap lai hoac de trong neu chua xac dinh.\n");
        }
    }
    
    printf("Nhap thong tin dieu tri thanh cong!\n");
}

//in thong tin benh nhan
void displayPxInfo(Node* node){
    if(node == NULL){
        printf("Khong co thong tin benh nhan.\n");
        return;
    }
    
    printf("\n===== THONG TIN BENH NHAN %s =====\n", node->px->name);
    printf("\tID: %s\n", node->px->pxID); 
    printf("\tHo ten: %s\n", node->px->name);
    printf("\tTuoi: %d\n", node->px->age);
    printf("\tGioi tinh: %s\n", node->px->gender);
    printf("\tDia chi: %s\n", node->px->address);
    printf("\tSo dien thoai: %s\n", node->px->contactNum);
    
    printf("\n----- CHAN DOAN BENH -----\n");
    printf("\tID chan doan: %s\n", node->dx->dxID);
    printf("\tChan doan: %s\n", node->dx->dx);
    printf("\tTrieu chung: %s\n", node->dx->sx);
    printf("\tNgay chan doan: %s\n", node->dx->dxDate);
    
    printf("\n----- THONG TIN DIEU TRI -----\n");
    printf("\tID dieu tri: %s\n", node->trt->trtID);
    printf("\tPhac do dieu tri: %s\n", node->trt->trtPlan);
    printf("\tDanh sach thuoc: %s\n", node->trt->meds);
    printf("\tLieu luong: %.2f\n", node->trt->dosage);
    printf("\tNgay bat dau dieu tri: %s\n", node->trt->trtStartDate);
    printf("\tNgay ket thuc dieu tri: %s\n", strlen(node->trt->trtEndDate) > 0 ? node->trt->trtEndDate : "Chua xac dinh.");
    printf("----------------------------\n");
}

//In ds benh nhan 
void displayAllPx(Node* head){
    if (head == NULL){
        printf("Danh sach benh nhan trong.\n");
        return;
    }
    
    printf("\n===== DANH SACH BENH NHAN =====\n");
    int cnt = 0;
    Node* now = head;
    
    while(now != NULL){
        cnt++;
        displayPxInfo(now);
        now = now->next;
    }
    
    printf("Tong so benh nhan: %d\n", cnt);
}

//Tim kiem benh nhan theoo Id hoac ten
void searchPx(Node* head){
    if(head == NULL){
        printf("Danh sach benh nhan trong.\n");
        return;
    }
    
    printf("\n===== TIM KIEM BENH NHAN =====\n");
    printf("1. Tim theo ID\n");
    printf("2. Tim theo ten\n");
    printf("Lua chon: ");
    
    int choice;
    if (scanf("%d", &choice) != 1){
        printf("Nhap sai dinh dang.\n");
        clearBuffer();
        return;
    }
    clearBuffer();
    
    if(choice == 1){
        //Tim theo ID
        char searchID[ID_SIZE];
        printf("Nhap ID benh nhan can tim: ");
        fgets(searchID, ID_SIZE, stdin);
        searchID[strcspn(searchID, "\n")] = '\0';
        
        Node* now = head;
        bool found = false;
        
        while (now != NULL){
            if (strcmp(now->px->pxID, searchID) == 0){
                displayPxInfo(now);
                found = true;
                break;
            }
            now = now->next;
        }
        
        if(!found){
            printf("Khong tim thay benh nhan co ID: %s\n", searchID);
        }
    } else if(choice == 2){
        //tim theo ten
        char searchName[50];
        printf("Nhap ten benh nhan can tim: ");
        fgets(searchName, 50, stdin);
        searchName[strcspn(searchName, "\n")] = '\0';
        
        Node* now = head;
        bool found = false;
        int cnt = 0;
        
        printf("\nKet qua tim kiem cho ten: %s\n", searchName);
        
        while(now != NULL){
            if (strstr(now->px->name, searchName) != NULL){
                printf("%d. ID: %s - Ho ten: %s - Tuoi: %d - Chan doan: %s\n", 
                      ++cnt, now->px->pxID, now->px->name, 
                      now->px->age, now->dx->dx);
                found = true;
            }
            now = now->next;
        }
        
        if (found){
            printf("Tim thay %d benh nhan.\n", cnt);
            
            //In in4 neu chi tim thay 1 benh nhan
            if(cnt == 1){
                printf("Ban co muon xem thong tin chi tiet? (1: Co, 0: Khong): ");
                int viewDetail;
                if (scanf("%d", &viewDetail) == 1 && viewDetail == 1){
                    clearBuffer();
                    //Tim lai va in benh nhan
                    now = head;
                    while(now != NULL){
                        if (strstr(now->px->name, searchName) != NULL){
                            displayPxInfo(now);
                            break;
                        }
                        now = now->next;
                    }
                } else{
                    clearBuffer();
                }
            }
        } else{
            printf("Khong tim thay benh nhan co ten: %s\n", searchName);
        }
    } else{
        printf("Lua chon khong hop le.\n");
    }
}

//Cap nhat in4 benh nhan
void updatePx(Node* head){
    if (head == NULL){
        printf("Danh sach benh nhan trong.\n"); return;
    }
    
    printf("\n===== CAP NHAT THONG TIN BENH NHAN =====\n");
    char searchID[ID_SIZE];
    printf("Nhap ID benh nhan can cap nhat: ");
    fgets(searchID, ID_SIZE, stdin);
    searchID[strcspn(searchID, "\n")] = '\0';
    
    Node* now = head;
    bool found = false;
    
    while(now != NULL){
        if (strcmp(now->px->pxID, searchID) == 0){
            found = true;
            break;
        }
        now = now->next;
    }
    
    if (!found){
        printf("Khong tim thay benh nhan co ID: %s\n", searchID); return;
    }
    
    printf("\nThong tin hien tai cua benh nhan:\n");
    displayPxInfo(now);
    
    printf("\nLua chon thong tin can cap nhat:\n");
    printf("1. Thong tin ca nhan\n");
    printf("2. Chan doan benh\n");
    printf("3. Thong tin dieu tri\n");
    printf("0. Quay lai\n");
    printf("Lua chon: ");
    
    int choice;
    if (scanf("%d", &choice) != 1){
        printf("Nhap sai dinh dang.\n");
        clearBuffer(); return;
    }
    clearBuffer();
    
    switch(choice){
        case 1:
            //Menu cap nhat in4
            printf("\n----- CAP NHAT THONG TIN CA NHAN -----\n");
            printf("1. Ho ten\n");
            printf("2. Tuoi\n");
            printf("3. Gioi tinh\n");
            printf("4. Dia chi\n");
            printf("5. So dien thoai\n");
            printf("0. Quay lai\n");
            printf("Lua chon: ");
            
            int subChoice;
            if (scanf("%d", &subChoice) != 1){
                printf("Nhap sai dinh dang.\n");
                clearBuffer(); return;
            }
            clearBuffer();
            
            switch(subChoice){
                case 1:
                    //Cap nhat ten
                    while(1){
                        printf("Nhap ho ten moi: ");
                        char tmpName[100];
                        fgets(tmpName, sizeof(tmpName), stdin);
                        tmpName[strcspn(tmpName, "\n")] = '\0';
                        
                        if (strlen(tmpName) == 0){
                            printf("Ten khong duoc de trong. Vui long nhap lai.\n"); continue;
                        }
                        
                        if (strlen(tmpName) >= sizeof(now->px->name)){
                            printf("Ten qua dai. Vui long nhap lai.\n"); continue;
                        }
                        
                        if (isName(tmpName)){
                            strcpy(now->px->name, tmpName);
                            printf("Cap nhat ten thanh cong!\n"); break;
                        }else{
                            printf("Ten khong hop le. Vui long nhap lai.\n");
                        }
                    }
                    break;
                case 2:
                    //Cap nhat tuoi
                    while(1){
                        printf("Nhap tuoi moi (%d-%d): ", MIN_AGE, MAX_AGE);
                        int newAge;
                        if (scanf("%d", &newAge) == 1){
                            if (newAge >= MIN_AGE && newAge <= MAX_AGE){
                                now->px->age = newAge;
                                printf("Cap nhat tuoi thanh cong!\n");
                                clearBuffer();
                                break;
                            } else{
                                printf("Tuoi khong hop le. Vui long nhap lai (tu %d den %d).\n", MIN_AGE, MAX_AGE);
                                clearBuffer();
                            }
                        } else{
                            printf("Nhap sai dinh dang. Vui long nhap lai.\n");
                            clearBuffer();
                        }
                    }
                    break;
                case 3:
                    //Cap nhat gioi tinh
                    while(1){
                        printf("Nhap gioi tinh moi (Nam/Nu): ");
                        char tmpGender[20];
                        fgets(tmpGender, sizeof(tmpGender), stdin);
                        tmpGender[strcspn(tmpGender, "\n")] = '\0';
                        
                        if (strcmp(tmpGender, "Nam") == 0 || strcmp(tmpGender, "Nu") == 0){
                            strcpy(now->px->gender, tmpGender);
                            printf("Cap nhat gioi tinh thanh cong!\n");
                            break;
                        } else{
                            printf("Gioi tinh khong hop le. Vui long nhap 'Nam' hoac 'Nu'.\n");
                        }
                    }
                    break;
                case 4:
                    //Cap nhat dia chi
                    while(1){
                        printf("Nhap dia chi moi: ");
                        char tmpAddress[150];
                        fgets(tmpAddress, sizeof(tmpAddress), stdin);
                        tmpAddress[strcspn(tmpAddress, "\n")] = '\0';
                        
                        if(strlen(tmpAddress) > 0){
                            if (strlen(tmpAddress) >= sizeof(now->px->address)){
                                printf("Dia chi qua dai. Vui long nhap lai.\n");
                                continue;
                            }
                            strcpy(now->px->address, tmpAddress);
                            printf("Cap nhat dia chi thanh cong!\n");
                            break;
                        }else{
                            printf("Dia chi khong duoc de trong. Vui long nhap lai.\n");
                        }
                    }
                    break;
                case 5:
                    //Cap nhat sdt
                    while(1){
                        printf("Nhap so dien thoai moi (toi da %d so): ", PHONE_SIZE-1);
                        char tmpPhone[PHONE_SIZE*2];
                        fgets(tmpPhone, sizeof(tmpPhone), stdin);
                        tmpPhone[strcspn(tmpPhone, "\n")] = '\0';
                        
                        if(strlen(tmpPhone) == 0){
                            printf("So dien thoai khong duoc de trong. Vui long nhap lai.\n");
                            continue;
                        }
                        
                        if(!isPhoneNum(tmpPhone)){
                            printf("So dien thoai chi duoc chua cac chu so. Vui long nhap lai.\n");
                            continue;
                        }
                        
                        if(strlen(tmpPhone) >= PHONE_SIZE){
                            printf("So dien thoai qua dai, chi duoc phep nhap toi da %d so. Vui long nhap lai.\n", PHONE_SIZE-1);
                            continue;
                        }
                        
                        strcpy(now->px->contactNum, tmpPhone);
                        printf("Cap nhat so dien thoai thanh cong!\n");
                        break;
                    }
                    break;
                case 0:
                    printf("Quay lai menu chinh.\n");
                    break;
                default:
                    printf("Lua chon khong hop le.\n");
            }
            break;
        case 2:
            //Menu cap nhat chuan doan benh
            printf("\n----- CAP NHAT CHAN DOAN BENH -----\n");
            printf("1. Chan doan\n");
            printf("2. Trieu chung\n");
            printf("3. Ngay chan doan\n");
            printf("0. Quay lai\n");
            printf("Lua chon: ");
            
            if (scanf("%d", &subChoice) != 1){
                printf("Nhap sai dinh dang.\n");
                clearBuffer();
                return;
            }
            clearBuffer();
            
            switch(subChoice){
                case 1:
                    //Cap nhat chuan doan 
                    while(1){
                        printf("Nhap chan doan moi:\n");
                        char tmpDx[DESCR];
                        fgets(tmpDx, DESCR, stdin);
                        tmpDx[strcspn(tmpDx, "\n")] = '\0';
                        
                        if(strlen(tmpDx) > 0){
                            strcpy(now->dx->dx, tmpDx);
                            printf("Cap nhat chan doan thanh cong!\n");
                            break;
                        } else{
                            printf("Chan doan benh khong duoc de trong. Vui long nhap lai.\n");
                        }
                    }
                    break;
                case 2:
                    //Cap nhat trieu chung 
                    while(1){
                        printf("Nhap trieu chung moi:\n");
                        char tmpSx[DESCR];
                        fgets(tmpSx, DESCR, stdin);
                        tmpSx[strcspn(tmpSx, "\n")] = '\0';
                        
                        if(strlen(tmpSx) > 0){
                            strcpy(now->dx->sx, tmpSx);
                            printf("Cap nhat trieu chung thanh cong!\n");
                            break;
                        } else{
                            printf("Trieu chung benh khong duoc de trong. Vui long nhap lai.\n");
                        }
                    }
                    break;
                case 3:
                    //Cap nhat ngay chuan doan 
                    while(1){
                        printf("Nhap ngay chan doan moi (DD/MM/YYYY): ");
                        char tmpDate[20];
                        fgets(tmpDate, sizeof(tmpDate), stdin);
                        tmpDate[strcspn(tmpDate, "\n")] = '\0';
                        
                        if (isDate(tmpDate)){
                            strcpy(now->dx->dxDate, tmpDate);
                            printf("Cap nhat ngay chan doan thanh cong!\n");
                            break;
                        } else{
                            printf("Dinh dang ngay khong hop le. Vui long nhap lai.\n");
                        }
                    }
                    break;
                case 0:
                    printf("Quay lai menu chinh.\n");
                    break;
                default:
                    printf("Lua chon khong hop le.\n");
            }
            break;
        case 3:
            //Cap nhat thong tin dieu tri 
            printf("\n----- CAP NHAT THONG TIN DIEU TRI -----\n");
            printf("1. Phac do dieu tri\n");
            printf("2. Danh sach thuoc\n");
            printf("3. Lieu luong thuoc\n");
            printf("4. Ngay bat dau dieu tri\n");
            printf("5. Ngay ket thuc dieu tri\n");
            printf("0. Quay lai\n");
            printf("Lua chon: ");
            
            if (scanf("%d", &subChoice) != 1){
                printf("Nhap sai dinh dang.\n");
                clearBuffer();
                return;
            }
            clearBuffer();
            
            switch(subChoice){
                case 1:
                    //Cap nhat phac do dieu tri
                    while(1){
                        printf("Nhap phac do dieu tri moi:\n");
                        char tmpPlan[DESCR];
                        fgets(tmpPlan, DESCR, stdin);
                        tmpPlan[strcspn(tmpPlan, "\n")] = '\0';
                        
                        if(strlen(tmpPlan) > 0){
                            strcpy(now->trt->trtPlan, tmpPlan);
                            printf("Cap nhat phac do dieu tri thanh cong!\n");
                            break;
                        }else{
                            printf("Phac do dieu tri khong duoc de trong. Vui long nhap lai.\n");
                        }
                    }
                    break;
                case 2:
                    //Cap nhat danh sach thuoc
                    while(1){
                        printf("Nhap danh sach thuoc moi:\n");
                        char tmpMeds[DESCR];
                        fgets(tmpMeds, DESCR, stdin);
                        tmpMeds[strcspn(tmpMeds, "\n")] = '\0';
                        
                        if(strlen(tmpMeds) > 0){
                            strcpy(now->trt->meds, tmpMeds);
                            printf("Cap nhat danh sach thuoc thanh cong!\n");
                            break;
                        } else{
                            printf("Danh sach thuoc khong duoc de trong. Vui long nhap lai.\n");
                        }
                    }
                    break;
                case 3:
                    //Cap nhat lieu luong thuoc
                    while(1){
                        printf("Nhap lieu luong thuoc moi: ");
                        float newDosage;
                        if (scanf("%f", &newDosage) == 1){
                            if (newDosage > 0){
                                now->trt->dosage = newDosage;
                                printf("Cap nhat lieu luong thuoc thanh cong!\n");
                                clearBuffer();
                                break;
                            }else{
                                printf("Lieu luong thuoc phai lon hon 0. Vui long nhap lai.\n");
                                clearBuffer();
                            }
                        } else{
                            printf("Nhap sai dinh dang. Vui long nhap lai.\n");
                            clearBuffer();
                        }
                    }
                    break;
                case 4:
                    //Cap nhat ngay dieu tri
                    while(1){
                        printf("Nhap ngay bat dau dieu tri moi (DD/MM/YYYY): ");
                        char tmpDate[20];
                        fgets(tmpDate, sizeof(tmpDate), stdin);
                        tmpDate[strcspn(tmpDate, "\n")] = '\0';
                        
                        if (isDate(tmpDate)){
                            strcpy(now->trt->trtStartDate, tmpDate);
                            printf("Cap nhat ngay bat dau dieu tri thanh cong!\n");
                            break;
                        } else{
                            printf("Dinh dang ngay khong hop le. Vui long nhap lai.\n");
                        }
                    }
                    break;
                case 5:
                    //Cap nhat ngay ket thuc dieu tri
                    while(1){
                        printf("Nhap ngay ket thuc dieu tri moi (DD/MM/YYYY, hoac de trong neu chua xac dinh): ");
                        char tmpDate[20];
                        fgets(tmpDate, sizeof(tmpDate), stdin);
                        tmpDate[strcspn(tmpDate, "\n")] = '\0';
                        
                        if (strlen(tmpDate) == 0 || isDate(tmpDate)){
                            strcpy(now->trt->trtEndDate, tmpDate);
                            printf("Cap nhat ngay ket thuc dieu tri thanh cong!\n");
                            break;
                        }else{
                            printf("Dinh dang ngay khong hop le. Vui long nhap lai hoac de trong neu chua xac dinh.\n");
                        }
                    }
                    break;
                case 0:
                    printf("Quay lai menu chinh.\n");
                    break;
                default:
                    printf("Lua chon khong hop le.\n");
            }
            break;
        case 0:
            printf("Quay lai menu chinh.\n");
            break;
        default:
            printf("Lua chon khong hop le.\n");
    }
}

//Xoa benh nhan
void deletePx(Node** head){
    if(*head == NULL){
        printf("Danh sach benh nhan trong.\n"); return;
    }
    
    printf("\n===== XOA BENH NHAN =====\n");
    char searchID[ID_SIZE];
    printf("Nhap ID benh nhan can xoa: ");
    fgets(searchID, ID_SIZE, stdin);
    searchID[strcspn(searchID, "\n")] = '\0';
    
    Node* now = *head;
    Node* prev = NULL;
    bool found = false;
    
    //Tim benh nhan can xoa
    while (now != NULL){
        if (strcmp(now->px->pxID, searchID) == 0){
            found = true; break;
        }
        prev = now;
        now = now->next;
    }
    
    if (!found){
        printf("Khong tim thay benh nhan co ID: %s\n", searchID); return;
    }
    
    //Hien thi thong tin benh nhan trc khi xoa
    printf("\nThong tin benh nhan sap xoa:\n");
    displayPxInfo(now);
    
    //Xac nhan xoa
    printf("Ban co chac chan muon xoa benh nhan nay? (1: Co, 0: Khong): ");
    int confirm;
    if (scanf("%d", &confirm) != 1 || confirm != 1){
        printf("Huy bo xoa benh nhan.\n");
        clearBuffer(); return;
    }
    clearBuffer();
    
    //Xoa
    if (prev == NULL){
        //Xoa node dau
        *head = now->next;
        if (now->next != NULL){
            now->next->prep = NULL;
        }
    }else{
        //Xoa node giua hoac cuoi
        prev->next = now->next;
        if (now->next != NULL){
            now->next->prep = prev;
        }
    }
    
    free(now->px);
    free(now->dx);
    free(now->trt);
    free(now);
    
    printf("Xoa benh nhan thanh cong!\n");
}

//Luu ds benh nhan vao file
void saveFile(Node* head, const char* fileName){
    FILE* file = fopen(fileName, "wb");
    if (file == NULL){
        printf("Khong the mo file de ghi.\n"); return;
    }
    
    Node* now = head;
    int cnt = 0;
    
    while(now != NULL){
        fwrite(now->px, sizeof(pxIn4), 1, file);
        fwrite(now->dx, sizeof(pxDx), 1, file);
        fwrite(now->trt, sizeof(trt), 1, file);
        cnt++;
        now = now->next;
    }
    
    fclose(file);
    printf("Da luu %d benh nhan vao file %s.\n", cnt, fileName);
}

//Doc ds benh nhan tu file
void loadFile(Node** head, const char* fileName){
    FILE* file = fopen(fileName, "rb");
    if (file == NULL){
        printf("Khong the mo file de doc.\n"); return;
    }
    
    //xoa ds benh nhan now
    freeMemory(head);
    
    pxIn4* px;
    pxDx* dx;
    trt* t;
    int cnt = 0;
    
    while(1){
        px = makePxIn4();
        dx = makePxDx();
        t = makeTrt();
        
        size_t r1 = fread(px, sizeof(pxIn4), 1, file);
        size_t r2 = fread(dx, sizeof(pxDx), 1, file);
        size_t r3 = fread(t, sizeof(trt), 1, file);
        
        if (r1 != 1 || r2 != 1 || r3 != 1){
            free(px);
            free(dx);
            free(t);
            break;
        }
        
        Node* newNode = makeNode(px, dx, t);
        insertSorted(head, newNode);
        cnt++;
    }
    
    fclose(file);
    printf("Da doc %d benh nhan tu file %s.\n", cnt, fileName);
}

//Menu thao tac voi file
void fileMenu(Node** head){
    printf("\n===== THAO TAC VOI FILE =====\n");
    printf("1. Luu du lieu vao file\n");
    printf("2. Doc du lieu tu file\n");
    printf("0. Quay lai\n");
    printf("Lua chon: ");
    
    int choice;
    scanf("%d", &choice);
    clearBuffer();
    
    char fileName[100];
    
    switch(choice){
        case 1:
            printf("Nhap ten file de luu: ");
            fgets(fileName, 100, stdin);
            fileName[strcspn(fileName, "\n")] = '\0';
            saveFile(*head, fileName);
            break;
        case 2:
            printf("Nhap ten file de doc: ");
            fgets(fileName, 100, stdin);
            fileName[strcspn(fileName, "\n")] = '\0';
            loadFile(head, fileName);
            break;
        case 0:
            return;
        default:
            printf("Lua chon khong hop le.\n");
    }
}
