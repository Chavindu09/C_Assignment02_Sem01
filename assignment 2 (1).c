#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_CANDIDATES 10
#define MAX_VOTERS     100

struct NICRecord {
    char nic[13];
    int  age;
    int  birthYear;
    int  day;
    char gender;
    char nicType;
};

struct Candidate {
    int  candidateID;
    char name[50];
    char party[50];
    char symbol[30];
    char district[30];
    int  voteCount;
};

struct Voter {
    int  voterID;
    char name[50];
    char id[13];
    int  age;
    char district[30];
    int  hasVoted;
};

int totalVotesCast = 0;

int validateNIC (struct NICRecord person);
struct NICRecord extractDetails (struct NICRecord person);
struct NICRecord determineGender (struct NICRecord person);
struct NICRecord calculateAge (struct NICRecord person);
struct NICRecord checkEligibility (struct NICRecord person);
void nicMenu (void);

void initializeCandidates (struct Candidate candidates[], int *count);
void displayAllCandidates (struct Candidate candidates[], int count);
void displayCandidateByID (struct Candidate candidates[], int count, int id);
void sortCandidatesByName (struct Candidate candidates[], int count);
void displayCandidateMenu (struct Candidate candidates[], int *count);

int  registerVoter (struct Voter voters[], int *voterCount);
void displayAllVoters (struct Voter voters[], int voterCount);
void searchVoterByNIC (struct Voter voters[], int voterCount, char *nic);
void displayVoterMenu (struct Voter voters[], int *voterCount);

int  findVoterByNIC (struct Voter voters[], int voterCount, char *id);
int  checkVoted (struct Voter voters[], int index);
int  castVote (struct Candidate candidates[], int candidateCount, int candidateID);
void updateVoteCount (struct Candidate candidates[], int index);
void printBallotHeader (void);
void displayVotingResults (struct Candidate candidates[], int count);
void votingMenu (struct Candidate candidates[], int candidateCount, struct Voter voters[],int *voterCount);

void displayMainMenu (void);

// MODULE 1 - NIC VALIDATION & ELIGIBILITY //

int validateNIC(struct NICRecord person)
{
    int len = (int)strlen(person.nic);

    if (len != 10 && len != 12) {
        printf("  ERROR: Invalid NIC length.\n");
        printf("         Old NIC must be 10 characters.\n");
        printf("         New NIC must be 12 characters.\n");
        return 0;
    }

    if (len == 10) {
        for (int i = 0; i < 9; i++) {
            if (person.nic[i] < '0' || person.nic[i] > '9') {
                printf("  ERROR: First 9 characters of old NIC must be digits.\n");
                return 0;
            }
        }
        char last = person.nic[9];
        if (last != 'V' && last != 'v' && last != 'X' && last != 'x') {
            printf("  ERROR: Old NIC must end with 'V' or 'X'.\n");
            return 0;
        }
    } else {
        for (int i = 0; i < 12; i++) {
            if (person.nic[i] < '0' || person.nic[i] > '9') {
                printf("  ERROR: New NIC must contain digits only.\n");
                return 0;
            }
        }
    }

    printf("  NIC is valid.\n");
    return 1;
}

struct NICRecord extractDetails(struct NICRecord person)
{
    int len = (int)strlen(person.nic);
    char buf[5];

    if (len == 10) {
        person.nicType = '0';
        buf[0] = person.nic[0]; buf[1] = person.nic[1]; buf[2] = '\0';
        person.birthYear = atoi(buf) + 1900;
        buf[0] = person.nic[2]; buf[1] = person.nic[3]; buf[2] = person.nic[4]; buf[3] = '\0';
        person.day = atoi(buf);
        printf("  NIC Type  : Old (10-character)\n");

    } else if (len == 12) {
        person.nicType = '1';
        buf[0] = person.nic[0]; buf[1] = person.nic[1];
        buf[2] = person.nic[2]; buf[3] = person.nic[3]; buf[4] = '\0';
        person.birthYear = atoi(buf);
        buf[0] = person.nic[4]; buf[1] = person.nic[5]; buf[2] = person.nic[6]; buf[3] = '\0';
        person.day = atoi(buf);
        printf("  NIC Type  : New (12-character)\n");

    } else {
        printf("  ERROR: Cannot extract details - invalid NIC.\n");
    }

    return person;
}

struct NICRecord determineGender(struct NICRecord person)
{
    if (person.day > 500) { person.gender = 'F'; person.day -= 500; }
    else                  { person.gender = 'M'; }
    printf("  Gender    : %s\n", (person.gender == 'F') ? "Female" : "Male");
    return person;
}

struct NICRecord calculateAge(struct NICRecord person)
{
    person.age = 2026 - person.birthYear;
    printf("  Birth Year: %d\n", person.birthYear);
    printf("  Age       : %d\n", person.age);
    return person;
}

struct NICRecord checkEligibility(struct NICRecord person)
{
    if (person.age >= 18) printf("  Eligibility: ELIGIBLE to vote.\n");
    else                  printf("  Eligibility: NOT eligible to vote (under 18).\n");
    return person;
}

void nicMenu(void)
{
    int choice;
    struct NICRecord person;

    do {
        printf("\n--------------------------------------------\n");
        printf("        NIC VALIDATION MODULE\n");
        printf("--------------------------------------------\n");
        printf("1. Validate a NIC\n");
        printf("2. Check Full Details & Eligibility\n");
        printf("0. Back to Main Menu\n");
        printf("--------------------------------------------\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter NIC Number: ");
                scanf("%12s", person.nic);
                validateNIC(person);
                break;
            case 2:
                printf("Enter NIC Number: ");
                scanf("%12s", person.nic);
                if (!validateNIC(person)) break;
                person = extractDetails(person);
                person = determineGender(person);
                person = calculateAge(person);
                person = checkEligibility(person);
                break;
            case 0:
                printf("  Returning to Main Menu...\n");
                break;
            default:
                printf("  Invalid choice. Please try again.\n");
        }
    } while (choice != 0);
}


// MODULE 2 - CANDIDATE MANAGEMENT //

void initializeCandidates(struct Candidate candidates[], int *count)
{
    candidates[0] = (struct Candidate){1,  "Ranil Wickremesinghe",     "United National Party",       "Elephant",    "Colombo",      0};
    candidates[1] = (struct Candidate){2,  "Sajith Premadasa",         "Samagi Jana Balawegaya",      "Swan",        "Hambantota",   0};
    candidates[2] = (struct Candidate){3,  "Anura Kumara Dissanayake", "National People's Power",     "Compass",     "Colombo",      0};
    candidates[3] = (struct Candidate){4,  "Dullas Alahapperuma",      "Sri Lanka Podujana Peramuna", "Betel Leaf",  "Matara",       0};
    candidates[4] = (struct Candidate){5,  "Champika Ranawaka",        "Pivithuru Hela Urumaya",      "Wheel",       "Gampaha",      0};
    candidates[5] = (struct Candidate){6,  "Mahinda Rajapaksa",        "Sri Lanka Freedom Party",     "Hand",        "Kurunegala",   0};
    candidates[6] = (struct Candidate){7,  "Rishard Bathiudeen",       "All Ceylon Makkal Congress",  "Star",        "Mannar",       0};
    candidates[7] = (struct Candidate){8,  "Mano Ganesan",             "Democratic People's Front",   "Rising Sun",  "Nuwara Eliya", 0};
    candidates[8] = (struct Candidate){9,  "Rauff Hakeem",             "Sri Lanka Muslim Congress",   "Crescent",    "Kandy",        0};
    candidates[9] = (struct Candidate){10, "Wimal Weerawansa",         "National Freedom Front",      "Key",         "Colombo",      0};

    *count = MAX_CANDIDATES;

}

void displayAllCandidates(struct Candidate candidates[], int count)
{
    printf("\n");
    printf("  --------------------------------------------------------------------------------\n");
    printf("                        REGISTERED CANDIDATES\n");
    printf("  --------------------------------------------------------------------------------\n");
    printf("  %-4s  %-26s  %-30s  %-12s  %-12s\n", "ID", "Name", "Party", "Symbol", "District");
    printf("  --------------------------------------------------------------------------------\n");
    for (int i = 0; i < count; i++) {
        printf("  %-4d  %-26s  %-30s  %-12s  %-12s\n",
               candidates[i].candidateID, candidates[i].name,
               candidates[i].party, candidates[i].symbol, candidates[i].district);
    }
    printf("  --------------------------------------------------------------------------------\n");
    printf("  Total Candidates: %d\n", count);
}

void displayCandidateByID(struct Candidate candidates[], int count, int id)
{
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (candidates[i].candidateID == id) {
            printf("\n  ----------------------------------------\n");
            printf("           CANDIDATE DETAILS\n");
            printf("  ----------------------------------------\n");
            printf("  Candidate ID : %d\n", candidates[i].candidateID);
            printf("  Name         : %s\n", candidates[i].name);
            printf("  Party        : %s\n", candidates[i].party);
            printf("  Symbol       : %s\n", candidates[i].symbol);
            printf("  District     : %s\n", candidates[i].district);
            printf("  Votes        : %d\n", candidates[i].voteCount);
            printf("  ----------------------------------------\n");
            found = 1; break;
        }
    }
    if (!found) printf("  Candidate with ID %d not found.\n", id);
}

void sortCandidatesByName(struct Candidate candidates[], int count)
{
    struct Candidate temp; int minIdx;
    for (int i = 0; i < count - 1; i++) {
        minIdx = i;
        for (int j = i + 1; j < count; j++)
            if (strcmp(candidates[j].name, candidates[minIdx].name) < 0) minIdx = j;
        temp = candidates[i]; candidates[i] = candidates[minIdx]; candidates[minIdx] = temp;
    }
    printf("\n  Candidates sorted alphabetically by name.\n");
    displayAllCandidates(candidates, count);
}

void displayCandidateMenu(struct Candidate candidates[], int *count)
{
    int choice, searchID;
    do {
        printf("\n--------------------------------------------\n");
        printf("      CANDIDATE MANAGEMENT MODULE\n");
        printf("--------------------------------------------\n");
        printf("1. Initialize / Reload Candidates\n");
        printf("2. Display All Candidates\n");
        printf("3. Search Candidate by ID\n");
        printf("4. Sort Candidates by Name\n");
        printf("0. Back to Main Menu\n");
        printf("--------------------------------------------\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: initializeCandidates(candidates, count); break;
            case 2: displayAllCandidates(candidates, *count); break;
            case 3:
                printf("Enter Candidate ID to search: ");
                scanf("%d", &searchID);
                displayCandidateByID(candidates, *count, searchID); break;
            case 4: sortCandidatesByName(candidates, *count); break;
            case 0: printf("  Returning to Main Menu...\n"); break;
            default: printf("  Invalid choice. Please try again.\n");
        }
    } while (choice != 0);
}

// MODULE 3 - VOTER REGISTRATION //

int registerVoter(struct Voter voters[], int *voterCount)
{
    if (*voterCount >= MAX_VOTERS) {
        printf("  ERROR: Voter register is full (max %d).\n", MAX_VOTERS);
        return 0;
    }

    struct Voter newVoter;
    struct NICRecord nicCheck;

    printf("  Enter Full Name       : ");
    scanf(" %49[^\n]", newVoter.name);

    printf("  Enter NIC Number      : ");
    scanf("%12s", newVoter.id);

    strcpy(nicCheck.nic, newVoter.id);

    if (!validateNIC(nicCheck)) {
        printf("  Registration failed: Invalid NIC.\n");
        return 0;
    }

    for (int i = 0; i < *voterCount; i++) {
        if (strcmp(voters[i].id, newVoter.id) == 0) {
            printf("  ERROR: This NIC is already registered (Voter ID: %d).\n", voters[i].voterID);
            return 0;
        }
    }

    nicCheck = extractDetails(nicCheck);
    nicCheck = calculateAge(nicCheck);

    if (nicCheck.age < 18) {
        printf("  Registration failed: Applicant is under 18 (Age: %d).\n", nicCheck.age);
        return 0;
    }

    printf("  Enter District        : ");
    scanf(" %29[^\n]", newVoter.district);

    newVoter.voterID  = *voterCount + 1;
    newVoter.age      = nicCheck.age;
    newVoter.hasVoted = 0;

    voters[*voterCount] = newVoter;
    (*voterCount)++;

    printf("\n  ----------------------------------------\n");
    printf("  Voter Registered Successfully!\n");
    printf("  Voter ID : %d\n",  newVoter.voterID);
    printf("  Name     : %s\n",  newVoter.name);
    printf("  NIC      : %s\n",  newVoter.id);
    printf("  Age      : %d\n",  newVoter.age);
    printf("  District : %s\n",  newVoter.district);
    printf("  ----------------------------------------\n");
    return 1;
}

void displayAllVoters(struct Voter voters[], int voterCount)
{
    if (voterCount == 0) { printf("  No voters registered yet.\n"); return; }

    printf("\n");
    printf("  ----------------------------------------------------------------------------\n");
    printf("                         REGISTERED VOTERS\n");
    printf("  ----------------------------------------------------------------------------\n");
    printf("  %-6s  %-24s  %-14s  %-4s  %-14s  %-6s\n",
           "VID", "Name", "NIC", "Age", "District", "Voted");
    printf("  ----------------------------------------------------------------------------\n");

    for (int i = 0; i < voterCount; i++) {
        printf("  %-6d  %-24s  %-14s  %-4d  %-14s  %-6s\n",
               voters[i].voterID, voters[i].name, voters[i].id,
               voters[i].age, voters[i].district,
               (voters[i].hasVoted == 1) ? "Yes" : "No");
    }

    printf("  ----------------------------------------------------------------------------\n");
    printf("  Total Registered Voters: %d\n", voterCount);
}

void searchVoterByNIC(struct Voter voters[], int voterCount, char *nic)
{
    for (int i = 0; i < voterCount; i++) {
        if (strcmp(voters[i].id, nic) == 0) {
            printf("\n  ----------------------------------------\n");
            printf("            VOTER DETAILS\n");
            printf("  ----------------------------------------\n");
            printf("  Voter ID  : %d\n", voters[i].voterID);
            printf("  Name      : %s\n", voters[i].name);
            printf("  NIC       : %s\n", voters[i].id);
            printf("  Age       : %d\n", voters[i].age);
            printf("  District  : %s\n", voters[i].district);
            printf("  Has Voted : %s\n", (voters[i].hasVoted == 1) ? "Yes" : "No");
            printf("  ----------------------------------------\n");
            return;
        }
    }
    printf("  Voter with NIC '%s' not found.\n", nic);
}

void displayVoterMenu(struct Voter voters[], int *voterCount)
{
    int  choice;
    char searchNIC[13];

    do {
        printf("\n--------------------------------------------\n");
        printf("       VOTER REGISTRATION MODULE\n");
        printf("--------------------------------------------\n");
        printf("1. Register New Voter\n");
        printf("2. Display All Voters\n");
        printf("3. Search Voter by NIC\n");
        printf("0. Back to Main Menu\n");
        printf("--------------------------------------------\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("\n  --- NEW VOTER REGISTRATION ---\n");
                registerVoter(voters, voterCount); break;
            case 2:
                displayAllVoters(voters, *voterCount); break;
            case 3:
                printf("Enter NIC to search: ");
                scanf("%12s", searchNIC);
                searchVoterByNIC(voters, *voterCount, searchNIC); break;
            case 0:
                printf("  Returning to Main Menu...\n"); break;
            default:
                printf("  Invalid choice. Please try again.\n");
        }
    } while (choice != 0);
}

// MODULE 4 - VOTING SYSTEM //

int findVoterByNIC(struct Voter voters[], int voterCount, char *id)
{
    for (int i = 0; i < voterCount; i++) {
        if (strcmp(voters[i].id, id) == 0) {
            printf("  Voter found: %s (Voter ID: %d)\n", voters[i].name, voters[i].voterID);
            return i;
        }
    }
    return -1;
}

int checkVoted(struct Voter voters[], int index)
{
    return (voters[index].hasVoted == 1) ? 1 : 0;
}

void updateVoteCount(struct Candidate candidates[], int index)
{
    candidates[index].voteCount++;
    totalVotesCast++;
}

int castVote(struct Candidate candidates[], int candidateCount, int candidateID)
{
    for (int i = 0; i < candidateCount; i++) {
        if (candidates[i].candidateID == candidateID) {
            updateVoteCount(candidates, i);
            return candidates[i].voteCount;
        }
    }
    return 0;
}

void printBallotHeader(void)
{
    printf("\n--------------------------------------------\n");
    printf("              VOTING BOOTH\n");
    printf("--------------------------------------------\n");
}

void displayVotingResults(struct Candidate candidates[], int count)
{
    printf("\n  ----------------------------------------\n");
    printf("          CURRENT VOTE TALLY\n");
    printf("  ----------------------------------------\n");
    for (int i = 0; i < count; i++)
        printf("  %-26s : %d vote(s)\n", candidates[i].name, candidates[i].voteCount);
    printf("  ----------------------------------------\n");
    printf("  Total votes cast: %d\n", totalVotesCast);
    printf("  ----------------------------------------\n");
}

void votingMenu(struct Candidate candidates[], int candidateCount,
                struct Voter voters[], int *voterCount)
{
    int  choice;
    char voterNIC[13];
    int  candidateID;

    do {
        printf("\n--------------------------------------------\n");
        printf("             VOTING MODULE\n");
        printf("--------------------------------------------\n");
        printf("1. Cast a Vote\n");
        printf("2. View Current Results\n");
        printf("0. Back to Main Menu\n");
        printf("--------------------------------------------\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: {
                printBallotHeader();

                printf("Enter your NIC: ");
                scanf("%12s", voterNIC);

                int voterIdx = findVoterByNIC(voters, *voterCount, voterNIC);
                if (voterIdx == -1) {
                    printf("  ERROR: NIC not found in voter register.\n");
                    printf("  Please register first via the Voter Registration module.\n");
                    break;
                }

                if (checkVoted(voters, voterIdx)) {
                    printf("  ERROR: You have already cast your ballot.\n");
                    break;
                }

                displayAllCandidates(candidates, candidateCount);
                printf("\nEnter the Candidate ID you wish to vote for: ");
                scanf("%d", &candidateID);

                int updatedCount = castVote(candidates, candidateCount, candidateID);
                if (updatedCount == 0) {
                    printf("  ERROR: Invalid Candidate ID. Vote not recorded.\n");
                } else {
                    voters[voterIdx].hasVoted = 1;
                    printf("\n  Vote successfully recorded.\n");
                    printf("  Candidate's total votes : %d\n", updatedCount);
                    printf("  Total votes cast so far : %d\n", totalVotesCast);
                }
                break;
            }
            case 2:
                displayVotingResults(candidates, candidateCount); break;
            case 0:
                printf("  Returning to Main Menu...\n"); break;
            default:
                printf("  Invalid choice. Please try again.\n");
        }
    } while (choice != 0);
}


// MAIN MENU & ENTRY POINT //

void displayMainMenu(void)
{
    printf("\n----------------------------------------------------\n");
    printf("|     SRI LANKA ELECTION MANAGEMENT SYSTEM        |\n");
    printf("----------------------------------------------------\n");
    printf("1. NIC Validation & Eligibility\n");
    printf("2. Candidate Management\n");
    printf("3. Voter Registration\n");
    printf("4. Voting\n");
    printf("0. Exit\n");
    printf("----------------------------------------------------\n");
    printf("Enter your choice: ");
}

int main(void)
{
    struct Candidate candidates[MAX_CANDIDATES];
    int candidateCount = 0;

    struct Voter voters[MAX_VOTERS];
    int voterCount = 0;

    initializeCandidates(candidates, &candidateCount);

    int choice;
    do {
        displayMainMenu();
        scanf("%d", &choice);

        switch (choice) {
            case 1: nicMenu(); break;
            case 2: displayCandidateMenu(candidates, &candidateCount); break;
            case 3: displayVoterMenu(voters, &voterCount); break;
            case 4: votingMenu(candidates, candidateCount, voters, &voterCount); break;
            case 0:
                printf("\nThank you for using the Election Management System. Goodbye!\n");
                break;
            default:
                printf("  Invalid choice. Please try again.\n");
        }
    } while (choice != 0);

    return 0;
}
