#include<iostream>
#include<vector>
#include<queue>
#include<fstream>
#include<algorithm>
#include<unistd.h>
using namespace std;

#define space "\t\t\t\t\t"
#define RESET       "\033[0m"  
#define RED         "\033[31m"
#define GREEN       "\033[32m"
#define YELLOW      "\033[33m"
#define BLUE        "\033[34m"
#define MAGENTA     "\033[35m"
#define CYAN        "\033[36m"
#define WHITE       "\033[37m"

#define BG_RED      "\033[41m"
#define BG_GREEN    "\033[42m"
#define BG_YELLOW   "\033[43m"
#define BG_BLUE     "\033[44m"

class process;
class core;
class resource;
class process_using_resource;
class batch;

class core{
    public:
        string coreName;
        int number;
        vector<batch>all_batches;
        batch* workingBatch;
        const process* workingProcessRef;
        process* workingProcessPtr;
        process_using_resource* workingProcessUsingResourcesPtr;
        core(string coreName){
            this->coreName=coreName;
            this->workingProcessPtr=nullptr;
            this->workingProcessRef=nullptr;
            this->workingProcessUsingResourcesPtr=nullptr;
        }
        void setBatch(vector<batch>&all_batches,int batchIndex){
            this->workingBatch=&all_batches[batchIndex];
        }
};

class process{
    public:
        bool isImp;
        string pid;
        int bt,memory,priority=-1;
        process(string pid,int bt,int memory,bool isImp,int priority=-1){
            this->pid=pid;
            this->bt=bt;
            this->memory=memory;
            this->isImp=isImp;
            if(isImp)this->priority=priority;
        }    
        process(int bt,string pid){
            // cout<<"Demo process created by core"<<endl;
            this->bt=bt;
            this->pid=pid;
        } 
        process(){} 
};

struct compMinBT{
    bool operator()(const process& a, const process& b){
        return a.bt > b.bt;
    }
};

class resource{
    public:
        string resource_name;
        int free_resources;
        resource(string resource_name,int free_resources){

            this->resource_name=resource_name;
            this->free_resources=free_resources;
            cout<<MAGENTA<<free_resources<<" : "<<resource_name<<"'s created" <<RESET<<endl;

        }
};  

class process_using_resource{
    
    public:
        string PId;
        int bt,memory;
        vector<int>need;
        vector<int>allocated;
        vector<vector<process_using_resource>>requests;
        process_using_resource(string PId,int bt,int memory,vector<int>&need,vector<int>&allocated){

            this->PId=PId;
            this->bt=bt;
            this->memory=memory;
            this->need=need;
            this->allocated=allocated;
            cout<<endl;
        }
};

struct compMinResReq{
    bool operator()(const process_using_resource& a, const process_using_resource& b){
        int t1=0,t2=0;
        for(int i=0;i<3;i++){
            t1+=a.need[i];
            t2-=b.need[i];
        }
        return t1 > t2;
    }
};

void clear() {
    cout << "\033[2J\033[1;1H"; // ANSI escape code to clear screen and move cursor to top-left
}

class batch{  
    public:
        string batch_name;
        vector<priority_queue<process,vector<process>,compMinBT>>system;
        vector<queue<process>>Queue;
        vector<priority_queue<process,vector<process_using_resource>,compMinResReq>>ProcessAllocation;
        int RR_row,RR_col;

        batch(string batch_name,int type){
            this->batch_name=batch_name;
            cout<<CYAN<<"Batch "<<batch_name<<" created with "<<RESET;
            if(type==1){
                system.resize(6);
                cout<<6<<" heaps"<<endl;
            }else if(type==2){
                this->RR_row=0;
                this->RR_col=0;
                Queue.resize(6);
                cout<<6<<" Queues"<<endl;
            }else if(type==3){
                Queue.resize(2);
                cout<<2<<" Queues"<<endl;
            } else if(type==4){
                ProcessAllocation.resize(2);
                cout<<2<<" heaps"<<endl;
            }
        }

        void add(process&one_process){
            cout<<YELLOW;
            if(this->batch_name=="SJF"){
                if(one_process.memory<=8){
                    cout<<"Stored "<<one_process.pid<<" in SJF batch (8KB)"<<endl;
                    system[0].push(one_process);
                } else if(one_process.memory<=16){
                    cout<<"Stored "<<one_process.pid<<" in SJF batch (16KB)"<<endl;
                    system[1].push(one_process);
                } else if(one_process.memory<=32){
                    cout<<"Stored "<<one_process.pid<<" in SJF batch (32KB)"<<endl;
                    system[2].push(one_process);
                } else if(one_process.memory<=64){
                    cout<<"Stored "<<one_process.pid<<" in SJF batch (64KB)"<<endl;
                    system[3].push(one_process);
                } else if(one_process.memory<=128){
                    cout<<"Stored "<<one_process.pid<<" in SJF batch (128KB)"<<endl;
                    system[4].push(one_process);
                } else if(one_process.memory<=256){
                    cout<<"Stored "<<one_process.pid<<" in SJF batch (256KB)"<<endl;
                    system[5].push(one_process);
                } else {
                    cout<<"Memory cannot be greater than 256kb"<<endl<<RESET;
                    return;
                }
            } else if(this->batch_name=="RR"){
                if(one_process.memory<=8){
                    cout<<"Stored "<<one_process.pid<<" in Round Robbin batch (16KB)"<<endl;
                    Queue[0].push(one_process);
                } else if(one_process.memory<=16){
                    cout<<"Stored "<<one_process.pid<<" in Round Robbin batch (16KB)"<<endl;
                    Queue[1].push(one_process);
                } else if(one_process.memory<=32){
                    cout<<"Stored "<<one_process.pid<<" in Round Robbin batch (16KB)"<<endl;
                    Queue[2].push(one_process);
                } else if(one_process.memory<=64){
                    cout<<"Stored "<<one_process.pid<<" in Round Robbin batch (16KB)"<<endl;
                    Queue[3].push(one_process);
                } else if(one_process.memory<=128){
                    cout<<"Stored "<<one_process.pid<<" in Round Robbin batch (16KB)"<<endl;
                    Queue[4].push(one_process);
                } else if(one_process.memory<=256){
                    cout<<"Stored "<<one_process.pid<<" in Round Robbin batch (16KB)"<<endl;
                    Queue[5].push(one_process);
                } else {
                    cout<<"Memory cannot be greater than 256kb"<<endl<<RESET;
                    return;
                }
            }
            cout<<RESET;
                // cout<<one_process.pid<<" added in the "<<batch_name<<" batch"<<endl;
        }

        void add_Imp(process&one_process){
            // cout<<one_process.pid<<" added in priorioty batch"<<endl;
            cout<<one_process.pid<<" is stored in priority bacth"<<endl;
            if(one_process.priority==1){
                Queue[0].push(one_process);
            }
            else if(one_process.priority==2) Queue[1].push(one_process);
        }

        void add_reqRes(process_using_resource&one_process){
            ProcessAllocation[0].push(one_process);
            // cout<<one_process.PId<<" is pushed in PR batch"<<endl;
        }
      
        bool alreadyWokring(core& receiver){
            return receiver.workingProcessPtr!=nullptr;
        }

      //logic for RR Priority remainning
        void assignProcess(core& receiver){
            if(!receiver.workingBatch){
                    cout<<"No batch is assigned to "<<receiver.coreName<<endl;
                    return;
            }
            string receiverBatchName=receiver.workingBatch->batch_name;
            // cout<<" in batch : "<<this->batch_name<<endl;
            if(receiverBatchName=="SJF"){
                if(!alreadyWokring(receiver)){
                    cout<<receiver.coreName<<" is not currently working"<<endl;
                    // cout<<"Call for process received form "<<receiver.coreName;
                    for(int i=0;i<6;i++){
                        // cout<<"In SJF"<<endl;
                        if(!system[i].empty()){
                            // cout<<"PQ not empty for i = "<<i<<endl;
                            process* p=new process(this->system[i].top().bt,this->system[i].top().pid);
                            // process p(this->system[i].top().bt,this->system[i].top().pid);
                            receiver.workingProcessPtr=p;
                            // cout<<"p.pid = "<<p.pid<<endl;
                            // cout<<"popping the process : "<<system[i].top().pid<<" from prioroity queue : "<<endl;
                            // cout<<"receiver.workingProcessPtr.pid = "<<receiver.workingProcessPtr->pid<<endl;
                            system[i].pop();
                            return;
                        } else {
                            // cout<<"Empty PQ for i = "<<i<<endl;
                        }
                    }
                    cout<<"No process is availaible in the batch"<<endl;
                    receiver.workingProcessPtr=nullptr;
                } else {
                    cout<<space<<RED<<receiver.coreName<<" is already wokring on a process : "<<receiver.workingProcessPtr->pid<<endl<<RESET;
                }
            } else if(receiverBatchName=="RR"){
                // if(!alreadyWokring(receiver)){
                    for(int i=0;i<6;i++){
                        // cout<<"Size of queue = "<<Queue.size()<<endl;
                        if(!Queue[i].empty()){
                            // cout<<"Queue no - "<<i+1<<" is not empty so assigning the topmost process to the "<<receiver.coreName<<endl;
                            receiver.workingProcessPtr = &Queue[i].front();
                            // cout<<receiver.coreName<<" is assinged to process with PID : "<<receiver.workingProcessPtr->pid<<endl;
                            if(Queue[i].front().bt!=1)Queue[i].push(Queue[i].front());
                            Queue[i].pop();
                            return;
                        } else {
                            // cout<<"Queue["<<i<<"] is empty"<<endl;
                        }
                    }
                // } else {
                    // cout<<receiver.coreName<<" is already wokring on a process : "<<receiver.workingProcessPtr->pid<<endl;
                // }
                    cout<<"No process is availaible in the RR batch"<<endl;
                    receiver.workingProcessPtr=nullptr;
            } else if(receiverBatchName=="Priority"){
                if(receiver.coreName=="core3"){
                    if(!Queue[0].empty()){
                        receiver.workingProcessPtr=&Queue[0].front();
                        Queue[0].push(Queue[0].front());
                        Queue[0].pop();
                        return;
                    } else {
                        cout<<"Highest priority Queue is empty so assigning mid prioroity processes to core3"<<endl;
                        if(!Queue[1].empty()){
                            receiver.workingProcessPtr=&Queue[1].front();
                            Queue[1].push(Queue[1].front());
                            Queue[1].pop();
                            return;
                        } else {
                            cout<<"Mid priority processes Queue is also empty"<<endl;
                        }
                    }
                } else if(receiver.coreName=="core4"){
                    if(!Queue[1].empty()){
                        receiver.workingProcessPtr=&Queue[1].front();
                        Queue[1].push(Queue[1].front());
                        Queue[1].pop();
                        return;
                    } else {
                        cout<<"Medium priority Queue is empty so assigning highest prioroity processes to core4"<<endl;
                        if(!Queue[0].empty()){
                            receiver.workingProcessPtr=&Queue[0].front();
                            Queue[0].push(Queue[0].front());
                            Queue[0].pop();
                            return;
                        } else {
                            cout<<"Mid priority processes Queue is also empty"<<endl;
                        }
                    }
                } 
                cout<<"No process is availaible in the Priority batch"<<endl;
                receiver.workingProcessPtr=nullptr;
            } else {
                cout<<"Invalid"<<endl;
            } 
        }
};

void clearScreen(){
    cout << "\033[2J\033[1;1H";
}

void store_processes(vector<process>&all_processes,vector<batch>&all_batches){
    for(int i=0;i<all_processes.size();i++){
        for(int j=0;j<2;j++){
            all_batches[j].add(all_processes[i]);
        }
        if(all_processes[i].isImp)all_batches[2].add_Imp(all_processes[i]);
    }
}

void store_processes_using_resources(vector<process_using_resource>&all_processes_using_resources,vector<batch>&all_batches){
    for(int i=0;i<all_processes_using_resources.size();i++){
        all_batches[3].add_reqRes(all_processes_using_resources[i]);
    }
}

void load_resources_from_file(vector<resource>&all_resources){
        fstream file;
        file.open("file_data/resources.txt",ios::in);
        if(file.is_open()){
            string resource_name;
            int k_resources_int_total;
            while(file>>resource_name>>k_resources_int_total){
                resource r(resource_name,k_resources_int_total);
                all_resources.push_back(r);
            }
            file.close();
        } else {
            cout<<"Error opening the file"<<endl;
        }
}

void load_normal_processes_from_file(vector<process>&all_procesees,string filename="processes.txt"){
    string path="file_data/";
    fstream file;
    file.open(path+filename,ios::in);
    if(file.is_open()){
        string PId;
        int bt,memory,priority=-1;
        bool isImp;
        while(file>>PId>>bt>>memory>>isImp>>priority){
            process p(PId,bt,memory,isImp,priority);
            all_procesees.push_back(p);
        }
        file.close();
    } else {
        cout<<"Error opening the processes.txt file"<<endl;
    }
}

void load_processes_using_resources_from_file(vector<resource>&all_resources,vector<process_using_resource>&all_processes_using_resources,string filename="processes_using_resources_no_deadlock.txt"){
    fstream file;
    string path="file_data/";
    file.open(path+filename,ios::in);
    if(file.is_open()){
        string PId;
        int bt,memory,total,allo;
        vector<int>total_need(all_resources.size()),allocated(all_resources.size());
        while(file>>PId>>bt>>memory>>total>>allo){
            int i=0;
           while(total!=0){
                total_need[i]=total%10;
                allocated[i]=allo%10;
                total/=10;
                allo/=10;
                i++;
           }
           reverse(total_need.begin(),total_need.end());
           reverse(allocated.begin(),allocated.end());
           process_using_resource p(PId,bt,memory,total_need,allocated); 
           all_processes_using_resources.push_back(p);
           cout<<p.PId<<" is fetched that will be using resources : "<<i<<" resources"<<endl;
        }
        file.close();
    } else {
        cout<<"Error opening the processes.txt file"<<endl;
    }
    cout<<"hi"<<endl;

}

void load_processes_from_file(vector<resource>&all_resources,vector<process>&all_procesees,vector<process_using_resource>&all_process_using_resources){

    load_normal_processes_from_file(all_procesees);
    load_processes_using_resources_from_file(all_resources,all_process_using_resources);

}

void load_from_file(vector<resource>&all_resources,vector<process>&all_processes,vector<process_using_resource>&all_processes_using_resources){

        load_resources_from_file(all_resources);
        load_processes_from_file(all_resources,all_processes,all_processes_using_resources);

}

void load_manually(vector<int>&k_needed,vector<int>&k_allocated,vector<resource>&all_resources,vector<process>&all_processes,vector<process_using_resource>&all_processes_using_resources){
    bool reqRes,isImp;
    int n_resources, k_resources_of_each,n,bt,memory,priority;
    string resource_name,PId;
    cout<<"Enter how many resources are there : ";
    cin>>n_resources;
    for(int i=0;i<n_resources;i++){
        cout<<"Enter name of resource no:"<<i+1<<" : ";
        cin>>resource_name; 
        cout<<"How many "<<resource_name<<"'s are present in total : ";
        cin>>k_resources_of_each;
        resource temp(resource_name,k_resources_of_each);
        all_resources.push_back(temp);
    }
    cout<<"Enter number of processes : ";
    cin>>n;
    vector<int>need,allocated;
    for(int i=0;i<n;i++){
        cout<<"Enter PId of process no-"<<i+1<<" : ";
        cin>>PId;
        cout<<"Does "<<PId<<" requires Resources ? \n1 : Yes 0 : No\nYour choice : ";
        cin>>reqRes;
        cout<<"Enter burst time of "<<PId<<" : ";
        cin>>bt;
        cout<<"Enter memory size of "<<PId<<" : ";
        cin>>memory;
        if(!reqRes){
            cout<<"Is "<<PId<<" an important process?\n1 : Yes 0 : No\nYour choice : ";
            cin>>isImp;
            if(isImp){
                cout<<"Set priority for "<<PId<<"\n1 : High 2 : Mid\n3Your choice :  ";
                cin>>priority;
                // process(string pid,int bt,int memory,bool isImp,int priority=-1){
                process temp(PId,bt,memory,isImp,priority);
                all_processes.push_back(temp);
            } else {
                process temp(PId,bt,memory,isImp);
                all_processes.push_back(temp);
            }
        } else {
            int temp;
            for(int j=0;j<all_resources.size();j++){
                cout<<"How many "<<all_resources[i].resource_name<<"'s are needed for "<<PId<<" : ";
                cin>>temp;
                k_needed.push_back(temp);
            }
            for(int j=0;j<all_resources.size();j++){
                cout<<"How many "<<all_resources[i].resource_name<<"'s are allocated to "<<PId<<" : ";
                cin>>temp;
                k_allocated.push_back(temp);
            }
            process_using_resource temp_p(PId,bt,memory,k_needed,k_allocated);
            all_processes_using_resources.push_back(temp_p);
            need.clear();
            allocated.clear();
        }
    }
}

void create_batches(vector<batch>&batches){
    batch SJF("SJF",1);
    batch SRTF("RR",2);
    batch Priority("Priority",3);
    batch Process_Resource("Process_Resource",4);
    batches.push_back(SJF);
    batches.push_back(SRTF);
    batches.push_back(Priority);
    batches.push_back(Process_Resource);
   
}

int getChoice(){
    int choice;
    cout<<"Enter your choice :\n1 : Enter data manually\n2 : Load Data\nYour choice : ";
    cin>>choice;
    return choice;
}

void create_cores(vector<core>&all_cores,vector<batch>&all_batches){
    string name="core";
    vector<int>batchIndex={0,1,2,2,3,3};
    for(int i=1;i<=6;i++){
        core temp(name+to_string(i));
        temp.setBatch(all_batches,batchIndex[i-1]);
        all_cores.push_back(temp);
    }
}

void store_everything_in_batches(vector<process>&all_processes,vector<process_using_resource>&all_processes_using_resources,vector<batch>&all_batches){

    store_processes(all_processes,all_batches);
    store_processes_using_resources(all_processes_using_resources,all_batches);
}

bool checkEmptyFile(string filename){
    string path="file_data/";
    ifstream emptyCheck(path+filename,ios::binary|ios::ate);
    if(!emptyCheck.is_open()){
        cout<<"Error openign the file : "<<filename<<endl;
        return true;
    }
    return emptyCheck.tellg()==0;
}
    
void clearFile(string filename){
    string path="file_data/";
    ofstream clearFile(path+filename,ios::out|ios::trunc);
    if(!clearFile.is_open()){
        cout<<"Filee not found to clear"<<endl;
    }
}

void runtime_normal_processes_fetching(vector<batch>&all_batches){
    string filename="RuntimeNewProcesses.txt";
    if(!checkEmptyFile(filename)){
        vector<process>all_processes;
        // clearScreen();
        load_normal_processes_from_file(all_processes,filename);
        store_processes(all_processes,all_batches);
        clearFile(filename);
        cout<<all_processes.size()<<" new processes entered in "<<filename<<" file"<<endl;
    }
}

void runtime_processes_using_resources_fetching(vector<batch>&all_batches,vector<resource>&all_resources){
    string filename="RuntimeNewProcessesUsingResources.txt";
    if(!checkEmptyFile(filename)){
        vector<process_using_resource>all_processes_using_resources;
        // clearScreen();
        load_processes_using_resources_from_file(all_resources,all_processes_using_resources,filename);
        store_processes_using_resources(all_processes_using_resources,all_batches);
        clearFile(filename);
        cout<<all_processes_using_resources.size()<<" new processes using resources entered in "<<filename<<" file"<<endl;
    }
}

void runtime_process_fetching(vector<batch>&all_batches,vector<resource>&all_resources){
    runtime_normal_processes_fetching(all_batches);
    runtime_processes_using_resources_fetching(all_batches,all_resources);

}

void runtime_urgent_processes_fetching(){
    string filename="urgentProcesses.txt";
    if(!checkEmptyFile(filename)){
        vector<process>all_processes;
        load_normal_processes_from_file(all_processes,filename);
        cout<<all_processes.size()<<" new urgent processes detected!"<<endl;
        clearFile(filename);
    }
}

void runtime_fetching(vector<batch>&all_batches,vector<resource>&all_resources){
    runtime_urgent_processes_fetching();
    runtime_process_fetching(all_batches,all_resources);
}

const process& SJF_take(batch& SJF){
    for(int i=0;i<6;i++){
        if(!SJF.system[i].empty())return SJF.system[i].top();
    }
    throw "SJF batch empty";
}

// void core1_work(core& core1,batch& SJF){
//     if(core1.workingProcess==nullptr){
//         try{          
//         }
//     }
//     if(core1.workingProcess!=nullptr){
//         core1.workingProcess->bt--;
//         cout<<"Core1 is wokring on : "<<core1.workingProcess->pid<<" bt remainnig : "<<core1.workingProcess->bt<<endl;
//     } else {
//         core1.workingProcess=SJF.
//     }
// }

void assignProcessesToCores(vector<batch>&all_batches,vector<core>&all_cores){
    vector<int>temp={0,1,2,2,3,3};
    for(int i=0;i<4;i++){
        all_batches[temp[i]].assignProcess(all_cores[i]);
        if(all_cores[i].workingProcessPtr!=nullptr){
            cout<<space<<CYAN<<all_cores[i].coreName<<" is working on : "<<all_cores[i].workingProcessPtr->pid<<endl<<RESET;
            all_cores[i].workingProcessPtr->bt--;
            if(all_cores[i].workingProcessPtr->bt==0){
                cout<<space<<GREEN<<"Process : "<<all_cores[i].workingProcessPtr->pid<<" is executed successfully"<<RESET<<endl;
                all_cores[i].workingProcessPtr=nullptr;
            }
        }
        // all_working_processes[i]=p;
    }
}

void work(vector<batch>&all_batches,vector<core>&all_cores,vector<resource>&all_resources,vector<process*>&all_wokring_processes){
    assignProcessesToCores(all_batches,all_cores);
}

void free_all_running_processes(core& core1){
    if(core1.workingProcessPtr)delete core1.workingProcessPtr;
}

int main(){
    bool reqRes,isImp;
    int bt,memory,priority,n,k_resources_of_each,n_resources;
    string PId,resource_name;
    vector<int>k_needed,k_allocated;    
    vector<resource>all_resources;
    vector<process>all_processes;
    vector<process_using_resource>all_processes_using_resources;
    vector<batch>all_batches;
    vector<core>all_cores;
    vector<string>filenames={""};
    vector<process*>all_working_processes(4,nullptr);

    create_batches(all_batches);
    create_cores(all_cores,all_batches);

    int choice=getChoice();
    if(choice==1){
        load_manually(k_needed,k_allocated,all_resources,all_processes,all_processes_using_resources);
    } else if(choice==2){
        load_from_file(all_resources,all_processes,all_processes_using_resources);
    }

    store_everything_in_batches(all_processes,all_processes_using_resources,all_batches);
    cout<<"Stored everythign in batches"<<endl;

    while(1){
        clear();
        cout<<space<<GREEN<<"Cores on the Job!"<<endl<<RESET;
        work(all_batches,all_cores,all_resources,all_working_processes);
        runtime_fetching(all_batches,all_resources);
        sleep(1);
        cout<<"Fetching processes on runtime"<<endl;
    }
    // work(all_batches,all_cores,all_resources,all_working_processes);

    // free_all_running_processes(all_working_processes);

}