#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure to store patient information
typedef struct {
    int patientID;
    char name[100];
    int age;
    char gender[10];
    char contactInfo[50];
    char insurance[50];
} Patient;

// Structure to store doctor information
typedef struct {
    int doctorID;
    char name[100];
    char specialty[50];
    char availability[50]; // For simplicity, just a string (e.g., "Mon-Fri: 9 AM - 5 PM")
} Doctor;

// Structure to store appointment information
typedef struct {
    int appointmentID;
    int patientID;
    int doctorID;
    char appointmentDate[20]; // Format: DD/MM/YYYY
    char appointmentTime[10]; // Format: HH:MM
} Appointment;

// Structure to store billing information
typedef struct {
    int billID;
    int patientID;
    float totalAmount;
    char insuranceStatus[20]; // Processed/Unprocessed
    char paymentStatus[20]; // Paid/Unpaid
} Billing;

// Structure to store nurse information
typedef struct {
    int nurseID;
    char name[100];
    char shift[50]; // For simplicity, just a string (e.g., "Mon-Fri: 9 AM - 5 PM")
} Nurse;

// Patient registration
void registerPatient(FILE *patientFile) {
    Patient newPatient;
    printf("Enter Patient Details:\n");
    printf("Name: ");
    fgets(newPatient.name, 100, stdin);
    newPatient.name[strcspn(newPatient.name, "\n")] = '\0'; // Remove newline character
    printf("Age: ");
    scanf("%d", &newPatient.age);
    getchar(); // to clear the newline character from input buffer
    printf("Gender: ");
    fgets(newPatient.gender, 10, stdin);
    newPatient.gender[strcspn(newPatient.gender, "\n")] = '\0'; // Remove newline character
    printf("Contact Info: ");
    fgets(newPatient.contactInfo, 50, stdin);
    newPatient.contactInfo[strcspn(newPatient.contactInfo, "\n")] = '\0'; // Remove newline character
    printf("Insurance Info: ");
    fgets(newPatient.insurance, 50, stdin);
    newPatient.insurance[strcspn(newPatient.insurance, "\n")] = '\0'; // Remove newline character

    // Assigning a unique patient ID
    fseek(patientFile, 0, SEEK_END);
    int patientID = 1;
    Patient temp;
    while (fscanf(patientFile, "%d,%99[^,],%d,%9[^,],%49[^,],%49[^\n]\n",
                   &temp.patientID, temp.name, &temp.age, temp.gender,
                   temp.contactInfo, temp.insurance) != EOF) {
        patientID++;
    }
    newPatient.patientID = patientID;

    // Save patient info to the file
    fprintf(patientFile, "%d,%s,%d,%s,%s,%s\n", newPatient.patientID, newPatient.name, newPatient.age, newPatient.gender, newPatient.contactInfo, newPatient.insurance);
    printf("Patient registered successfully with ID: %d\n", newPatient.patientID);
    fflush(patientFile); // Ensure data is written to the file immediately
}

// Schedule appointment
void scheduleAppointment(FILE *appointmentFile, int patientID, int doctorID) {
    Appointment newAppointment;
    newAppointment.patientID = patientID;
    newAppointment.doctorID = doctorID;

    printf("Enter appointment date (DD/MM/YYYY): ");
    scanf("%s", newAppointment.appointmentDate);
    printf("Enter appointment time (HH:MM): ");
    scanf("%s", newAppointment.appointmentTime);

    // Assigning a unique appointment ID
    fseek(appointmentFile, 0, SEEK_END);
    int appointmentID = 1;
    Appointment temp;
    while (fscanf(appointmentFile, "%d,%d,%d,%19[^,],%9[^\n]\n",
                   &temp.appointmentID, &temp.patientID, &temp.doctorID,
                   temp.appointmentDate, temp.appointmentTime) != EOF) {
        appointmentID++;
    }
    newAppointment.appointmentID = appointmentID;

    // Save appointment info to the file
    fprintf(appointmentFile, "%d,%d,%d,%s,%s\n", newAppointment.appointmentID, newAppointment.patientID, newAppointment.doctorID, newAppointment.appointmentDate, newAppointment.appointmentTime);
    printf("Appointment scheduled successfully with ID: %d\n", newAppointment.appointmentID);
    fflush(appointmentFile); // Ensure data is written to the file immediately
}

// View patient record
void viewPatientRecord(FILE *patientFile, int patientID) {
    Patient patient;
    fseek(patientFile, 0, SEEK_SET); // Move the file pointer to the beginning of the file
    int found = 0; // Flag to check if the patient is found

    // Print a debug message to ensure file reading is working
    printf("Searching for Patient ID: %d\n", patientID);

    while (fscanf(patientFile, "%d,%99[^,],%d,%9[^,],%49[^,],%49[^\n]\n",
                   &patient.patientID, patient.name, &patient.age, patient.gender,
                   patient.contactInfo, patient.insurance) != EOF) {
        // Debug: print patient data
        printf("Read Patient ID: %d\n", patient.patientID);

        if (patient.patientID == patientID) {
            // Patient found, print details
            printf("Patient ID: %d\n", patient.patientID);
            printf("Name: %s\n", patient.name);
            printf("Age: %d\n", patient.age);
            printf("Gender: %s\n", patient.gender);
            printf("Contact Info: %s\n", patient.contactInfo);
            printf("Insurance Info: %s\n", patient.insurance);
            found = 1; // Set flag to true
            break; // Exit the loop as we found the patient
        }
    }
    if (!found) {
        printf("Patient with ID %d not found.\n", patientID);
    }
}

// Generate invoice
void generateInvoice(FILE *billingFile, int patientID, float totalAmount, const char *insuranceStatus) {
    Billing newBill;
    newBill.patientID = patientID;
    newBill.totalAmount = totalAmount;
    strcpy(newBill.insuranceStatus, insuranceStatus);
    strcpy(newBill.paymentStatus, "Unpaid");

    // Assigning a unique bill ID
    fseek(billingFile, 0, SEEK_END);
    int billID = 1;
    Billing temp;
    while (fscanf(billingFile, "%d,%d,%f,%19[^,],%19[^\n]\n",
                   &temp.billID, &temp.patientID, &temp.totalAmount,
                   temp.insuranceStatus, temp.paymentStatus) != EOF) {
        billID++;
    }
    newBill.billID = billID;

    // Save billing info to the file
    fprintf(billingFile, "%d,%d,%.2f,%s,%s\n", newBill.billID, newBill.patientID, newBill.totalAmount, newBill.insuranceStatus, newBill.paymentStatus);
    printf("Invoice generated successfully with Bill ID: %d\n", newBill.billID);
    fflush(billingFile); // Ensure data is written to the file immediately
}

// Register doctor
void registerDoctor(FILE *doctorFile) {
    Doctor newDoctor;
    printf("Enter Doctor Details:\n");
    printf("Name: ");
    fgets(newDoctor.name, 100, stdin);
    newDoctor.name[strcspn(newDoctor.name, "\n")] = '\0'; // Remove newline character
    printf("Specialty: ");
    fgets(newDoctor.specialty, 50, stdin);
    newDoctor.specialty[strcspn(newDoctor.specialty, "\n")] = '\0'; // Remove newline character
    printf("Availability: ");
    fgets(newDoctor.availability, 50, stdin);
    newDoctor.availability[strcspn(newDoctor.availability, "\n")] = '\0'; // Remove newline character

    // Assign doctor ID
    fseek(doctorFile, 0, SEEK_END);
    int doctorID = 1;
    Doctor temp;
    while (fscanf(doctorFile, "%d,%99[^,],%49[^,],%49[^\n]\n",
                   &temp.doctorID, temp.name, temp.specialty, temp.availability) != EOF) {
        doctorID++;
    }
    newDoctor.doctorID = doctorID;

    // Save doctor info to the file
    fprintf(doctorFile, "%d,%s,%s,%s\n", newDoctor.doctorID, newDoctor.name, newDoctor.specialty, newDoctor.availability);
    printf("Doctor registered successfully with ID: %d\n", newDoctor.doctorID);
    fflush(doctorFile); // Ensure data is written to the file immediately
}

// Register nurse
void registerNurse(FILE *nurseFile) {
    Nurse newNurse;
    printf("Enter Nurse Details:\n");
    printf("Name: ");
    fgets(newNurse.name, 100, stdin);
    newNurse.name[strcspn(newNurse.name, "\n")] = '\0'; // Remove newline character
    printf("Shift: ");
    fgets(newNurse.shift, 50, stdin);
    newNurse.shift[strcspn(newNurse.shift, "\n")] = '\0'; // Remove newline character

    // Assign nurse ID
    fseek(nurseFile, 0, SEEK_END);
    int nurseID = 1;
    Nurse temp;
    while (fscanf(nurseFile, "%d,%99[^,],%49[^\n]\n",
                   &temp.nurseID, temp.name, temp.shift) != EOF) {
        nurseID++;
    }
    newNurse.nurseID = nurseID;

    // Save nurse info to the file
    fprintf(nurseFile, "%d,%s,%s\n", newNurse.nurseID, newNurse.name, newNurse.shift);
    printf("Nurse registered successfully with ID: %d\n", newNurse.nurseID);
    fflush(nurseFile); // Ensure data is written to the file immediately
}

// Assign nurse to appointment
void assignNurseToAppointment(FILE *appointmentFile, FILE *nurseFile, int appointmentID, int nurseID) {
    Appointment tempAppointment;
    fseek(appointmentFile, 0, SEEK_SET);
    int found = 0;

    // Search for the appointment by ID
    while (fscanf(appointmentFile, "%d,%d,%d,%19[^,],%9[^\n]\n",
                   &tempAppointment.appointmentID, &tempAppointment.patientID,
                   &tempAppointment.doctorID, tempAppointment.appointmentDate,
                   tempAppointment.appointmentTime) != EOF) {
        if (tempAppointment.appointmentID == appointmentID) {
            found = 1;
            break;
        }
    }

    if (found) {
        // Add nurse assignment logic here
        printf("Assigning Nurse to Appointment ID: %d\n", appointmentID);
        Nurse nurse;
        fseek(nurseFile, 0, SEEK_SET);
        while (fscanf(nurseFile, "%d,%99[^,],%49[^\n]\n",
                       &nurse.nurseID, nurse.name, nurse.shift) != EOF) {
            if (nurse.nurseID == nurseID) {
                printf("Nurse %s assigned to Appointment ID: %d\n", nurse.name, appointmentID);
                break;
            }
        }
    } else {
        printf("Appointment ID %d not found.\n", appointmentID);
    }
}

// Generate report (optional)
void generateReport(FILE *patientFile, FILE *appointmentFile, FILE *billingFile) {
    printf("Generating report...\n");
    // This could generate summaries, totals, etc. based on data in the files.
    // Example: You can read through patient, appointment, and billing data to produce reports.
    // Implementation of report generation logic here.
}

int main() {
    FILE *patientFile = fopen("patients.txt", "a+"); // Open in append-read mode
    FILE *doctorFile = fopen("doctors.txt", "a+");
    FILE *appointmentFile = fopen("appointments.txt", "a+");
    FILE *billingFile = fopen("billing.txt", "a+");
    FILE *nurseFile = fopen("nurses.txt", "a+"); // New file for nurses

    if (!patientFile || !doctorFile || !appointmentFile || !billingFile || !nurseFile) {
        printf("Error opening file!\n");
        return -1;
    }

    int choice;
    while (1) {
        printf("\n--- Hospital Management System ---\n");
        printf("1. Register Patient\n");
        printf("2. Schedule Appointment\n");
        printf("3. View Patient Record\n");
        printf("4. Generate Invoice\n");
        printf("5. Register Doctor\n");
        printf("6. Generate Report\n");
        printf("7. Register Nurse\n");  // New option for registering nurse
        printf("8. Assign Nurse to Appointment\n");  // New option for assigning nurse
        printf("9. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // to clear the newline character from input buffer

        switch (choice) {
            case 1:
                registerPatient(patientFile);
                break;
            case 2: {
                int patientID, doctorID;
                printf("Enter Patient ID: ");
                scanf("%d", &patientID);
                printf("Enter Doctor ID: ");
                scanf("%d", &doctorID);
                scheduleAppointment(appointmentFile, patientID, doctorID);
                break;
            }
            case 3: {
                int patientID;
                printf("Enter Patient ID to view: ");
                scanf("%d", &patientID);
                viewPatientRecord(patientFile, patientID);
                break;
            }
            case 4: {
                int patientID;
                float totalAmount;
                char insuranceStatus[20];
                printf("Enter Patient ID for Invoice: ");
                scanf("%d", &patientID);
                printf("Enter total amount: ");
                scanf("%f", &totalAmount);
                getchar(); // to clear the newline character
                printf("Enter insurance status (Processed/Unprocessed): ");
                fgets(insuranceStatus, 20, stdin);
                insuranceStatus[strcspn(insuranceStatus, "\n")] = '\0'; // Remove newline
                generateInvoice(billingFile, patientID, totalAmount, insuranceStatus);
                break;
            }
            case 5:
                registerDoctor(doctorFile);
                break;
            case 6:
                generateReport(patientFile, appointmentFile, billingFile);
                break;
            case 7:
                registerNurse(nurseFile);  // Call to register a nurse
                break;
            case 8: {
                int appointmentID, nurseID;
                printf("Enter Appointment ID: ");
                scanf("%d", &appointmentID);
                printf("Enter Nurse ID: ");
                scanf("%d", &nurseID);
                assignNurseToAppointment(appointmentFile, nurseFile, appointmentID, nurseID);
                break;
            }
            case 9:
                fclose(patientFile);
                fclose(doctorFile);
                fclose(appointmentFile);
                fclose(billingFile);
                fclose(nurseFile);  // Close nurse file
                exit(0);
        }
    }
    return 0;
}
