#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CUSTOMERS 100000

// Data table
typedef struct {
      int customerID;
      int billingID;
      char lastName[16];
      char firstName[16];
} Customer;

// CustomerID Index table
typedef struct {
      int customerID;
      int position;
} CustomerSearchEntry;

// BillingID Index table
typedef struct {
      int billingID;
      int position;
} BillingSearchEntry;

// Reading file name
char* filePath() {
      char *file_name = malloc(21 * sizeof(char)); 
      if (!file_name) {
            printf("Memory allocation failed\n");
            return NULL;
      }
      printf("Enter filename (e.g., data.txt) located in the same folder as this program:\n");
      scanf("%20s", file_name); 

      return file_name;
}

//  Swap structure positions 
void swap(void *a, void *b, size_t size) {
      char tmp[size];
      memcpy(tmp, a, size);
      memcpy(a, b, size);
      memcpy(b, tmp, size);
}

// Binnary search 
int binarySearch(void *array, int size, int target, int mode) {
      int low = 0;
      int high = size - 1;
      while (low <= high) {
            int mid = (low + high) / 2;
            int key = mode == 1 ? ((CustomerSearchEntry*)array)[mid].customerID : ((BillingSearchEntry*)array)[mid].billingID;
            if (key < target){
                  low = mid + 1;
            }else if (key > target){
                  high = mid - 1;
            }else{
                  return mid;
            }
      }
      return -1;
}

int main() {
      // Check file path
      char *file_to_read = filePath();
      if (file_to_read == NULL) {
            return 1;
      }

      // Open input file 
      FILE *file_in = fopen(file_to_read, "r");
      if (!file_in) {
            printf("Failed to open input file\n");
            free(file_to_read);
            return 1;
      }

      // Alocating memory for struct Customer
      Customer *str = malloc(sizeof(Customer) * MAX_CUSTOMERS); 
      if (!str) {
            printf("Customer memory allocation failed\n");
            fclose(file_in);
            return 1;
      }

      // Alocating memory for struct CustomerSearchEntry
      CustomerSearchEntry *customerIndex = malloc(sizeof(CustomerSearchEntry) * MAX_CUSTOMERS); 
      if (!str) {
            printf("CustomerSearchEntry memory allocation failed\n");
            fclose(file_in);
            return 1;
      }

      // Alocating memory for struct BillingSearchEntry
      BillingSearchEntry *billingIndex = malloc(sizeof(BillingSearchEntry) * MAX_CUSTOMERS); 
      if (!str) {
            printf("BillingSearchEntry memory allocation failed\n");
            fclose(file_in);
            return 1;
      }

      // Write output file 
      int count = 0;
      while (fscanf(file_in, "%d %d %16s %16s", &str[count].customerID, &str[count].billingID, str[count].firstName, str[count].lastName) == 4 && count < MAX_CUSTOMERS) {
            customerIndex[count].position = count;
            customerIndex[count].customerID = str[count].customerID;
            billingIndex[count].position = count;
            billingIndex[count].billingID = str[count].billingID;
            count++;
      }
      fclose(file_in);

      // Selection sort algorithm Customers ID
      printf("Sorting Customers ID...\n");
      for (int i = 0; i < count; i++) {
            for (int j = i + 1; j < count; j++) {
                  if (customerIndex[j].customerID < customerIndex[i].customerID) {
                  swap(&customerIndex[i], &customerIndex[j], sizeof(customerIndex));
                  }
            }
      }

       // Selection sort algorithm Billing ID
      printf("Sorting Billing ID...\n");
      for (int i = 0; i < count; i++) {
            for (int j = i + 1; j < count; j++) {
                  if (billingIndex[j].billingID < billingIndex[i].billingID) {
                  swap(&billingIndex[i], &billingIndex[j], sizeof(billingIndex));
                  }
            }
      }

      // Main loop      
      int choice;
      do {
            printf("Press 1 to search by customer number.\n");
            printf("Press 2 to search by billing number.\n");
            printf("Press 0 to exit.\n");
            scanf("%d", &choice);

            switch (choice) {
                  case 1:
                  int custID;
                  printf("Enter customer number: ");
                  scanf("%d", &custID);
                  int idx = binarySearch(customerIndex, MAX_CUSTOMERS, custID, 1);
                  if (idx != -1) {
                        int pos = customerIndex[idx].position;
                        printf("FIRST NAME: %s\nLAST NAME: %s\nCUSTOMER NUMBER: %d\nBILLING NUMBER: %d\n",
                              str[pos].firstName, str[pos].lastName,
                              str[pos].customerID, str[pos].billingID);
                  } else {
                        printf("Customer not found.\n");
                  }
                  break;
                  case 2:
                  int billID;
                  printf("Enter billing number: ");
                  scanf("%d", &billID);
                  idx = binarySearch(billingIndex, MAX_CUSTOMERS, billID, 2);
                  if (idx != -1) {
                        int pos = billingIndex[idx].position;
                        printf("FIRST NAME: %s\nLAST NAME: %s\nCUSTOMER NUMBER: %d\nBILLING NUMBER: %d\n",
                              str[pos].firstName, str[pos].lastName,
                              str[pos].customerID, str[pos].billingID);
                  } else {
                        printf("Billing not found.\n");
                  }
                  break;
                  case 0:
                  printf("Goodbye.\n");
                  break;
            }
      } while (choice != 0);

      // Free allocated memory
      free(str);
      free(customerIndex);
      free(billingIndex);

      return 0;
}