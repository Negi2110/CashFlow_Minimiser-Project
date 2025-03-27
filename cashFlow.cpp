#include <bits/stdc++.h>
using namespace std;

class bank {
public:
    string name;
    int netAmount;
    set<string> types;
};

// Priority queues to store debtors & creditors
priority_queue<pair<int, int>> maxHeap;  // Stores {netAmount, index} (max-heap for creditors)
priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> minHeap; // Stores {netAmount, index} (min-heap for debtors)

void initializeHeaps(bank listOfNetAmounts[], int numBanks) {
    while (!maxHeap.empty()) maxHeap.pop();
    while (!minHeap.empty()) minHeap.pop();

    for (int i = 0; i < numBanks; i++) {
        if (listOfNetAmounts[i].netAmount > 0)
            maxHeap.push({listOfNetAmounts[i].netAmount, i});
        else if (listOfNetAmounts[i].netAmount < 0)
            minHeap.push({listOfNetAmounts[i].netAmount, i});
    }
}

void printAns(vector<vector<pair<int, string>>> &ansGraph, int numBanks, bank input[]) {
    cout << "\nThe transactions for minimum cash flow are as follows:\n\n";
    for (int i = 0; i < numBanks; i++) {
        for (int j = 0; j < numBanks; j++) {
            if (ansGraph[i][j].first != 0) {
                cout << input[i].name << " pays Rs " << ansGraph[i][j].first
                     << " to " << input[j].name << " via " << ansGraph[i][j].second << endl;
            }
        }
    }
    cout << "\n";
}

void minimizeCashFlow(int numBanks, bank input[], unordered_map<string, int> &indexOf, 
                      int numTransactions, vector<vector<int>> &graph, int maxNumTypes) {
    
    bank listOfNetAmounts[numBanks];
    
    for (int b = 0; b < numBanks; b++) {
        listOfNetAmounts[b].name = input[b].name;
        listOfNetAmounts[b].types = input[b].types;

        int amount = 0;
        for (int i = 0; i < numBanks; i++) amount += graph[i][b];  // Incoming
        for (int j = 0; j < numBanks; j++) amount -= graph[b][j];  // Outgoing

        listOfNetAmounts[b].netAmount = amount;
    }

    vector<vector<pair<int, string>>> ansGraph(numBanks, vector<pair<int, string>>(numBanks, {0, ""}));
    initializeHeaps(listOfNetAmounts, numBanks);
    
    while (!minHeap.empty() && !maxHeap.empty()) {
        auto [minAmount, minIndex] = minHeap.top();  // Most indebted bank
        auto [maxAmount, maxIndex] = maxHeap.top();  // Wealthiest bank
        minHeap.pop();
        maxHeap.pop();

        int transactionAmount = min(abs(minAmount), maxAmount);
        ansGraph[minIndex][maxIndex] = {transactionAmount, *(input[minIndex].types.begin())};

        listOfNetAmounts[minIndex].netAmount += transactionAmount;
        listOfNetAmounts[maxIndex].netAmount -= transactionAmount;

        if (listOfNetAmounts[minIndex].netAmount < 0)
            minHeap.push({listOfNetAmounts[minIndex].netAmount, minIndex});
        if (listOfNetAmounts[maxIndex].netAmount > 0)
            maxHeap.push({listOfNetAmounts[maxIndex].netAmount, maxIndex});
    }
    
    printAns(ansGraph, numBanks, input);
}

int main() {
    cout << "\n\t\t\t********************* Welcome to CASH FLOW MINIMIZER SYSTEM ***********************\n\n\n";
    cout << "This system minimizes the number of transactions among multiple banks in the world using different payment modes.\n\n";
    
    cout << "Enter the number of banks participating in the transactions: ";
    int numBanks;
    cin >> numBanks;
    
    bank input[numBanks];
    unordered_map<string, int> indexOf;
    
    cout << "Enter the bank details:\n";
    cout << "Format: BankName NumberOfPaymentModes PaymentModes\n";
    
    int maxNumTypes;
    for (int i = 0; i < numBanks; i++) {
        cout << "Bank " << i << " : ";
        cin >> input[i].name;
        indexOf[input[i].name] = i;
        int numTypes;
        cin >> numTypes;
        
        if (i == 0) maxNumTypes = numTypes;
        
        string type;
        while (numTypes--) {
            cin >> type;
            input[i].types.insert(type);
        }
    }
    
    cout << "Enter the number of transactions: ";
    int numTransactions;
    cin >> numTransactions;
    
    vector<vector<int>> graph(numBanks, vector<int>(numBanks, 0));
    
    cout << "Enter the transactions in the format: DebtorBank CreditorBank Amount\n";
    for (int i = 0; i < numTransactions; i++) {
        cout << "Transaction " << i << ": ";
        string s1, s2;
        int amount;
        cin >> s1 >> s2 >> amount;
        graph[indexOf[s1]][indexOf[s2]] = amount;
    }
    
    minimizeCashFlow(numBanks, input, indexOf, numTransactions, graph, maxNumTypes);
    return 0;
}
