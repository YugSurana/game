import java.util.*;

// ===== INTERFACE: TRANSACTABLE =====
interface Transactable {
    void deposit(double amount);
    void withdraw(double amount);
    double calculateInterestOrCharges();
}

// ===== INTERFACE: SEARCHABLE =====
interface Searchable {
    boolean matchesAccountNumber(String accNum);
    void displayDetails();
}

// ===== CUSTOM EXCEPTIONS =====
class AccountNotFoundException extends Exception {
    public AccountNotFoundException(String msg) {
        super(msg);
    }
}

class InsufficientBalanceException extends Exception {
    public InsufficientBalanceException(String msg) {
        super(msg);
    }
}

// ===== BASE CLASS: ACCOUNT =====
abstract class Account implements Transactable, Searchable {

    private String accountNumber;
    private String accountHolderName;
    private String branchName;
    protected double balance;

    private double[] recentTransactions;
    private int transactionCount;

    public Account(String accNum, String holderName, String branch, double initialBalance) {
        this.accountNumber = accNum;
        this.accountHolderName = holderName;
        this.branchName = branch;
        this.balance = initialBalance;

        recentTransactions = new double[10];
        transactionCount = 0;
    }

    @Override
    public void deposit(double amount) {
        balance += amount;
        recordTransaction(amount);
    }

    @Override
    public void withdraw(double amount) {
        balance -= amount;
        recordTransaction(-amount);
    }

    public void recordTransaction(double amount) {
        recentTransactions[transactionCount % 10] = amount;
        transactionCount++;
    }

    @Override
    public boolean matchesAccountNumber(String accNum) {
        return this.accountNumber.equals(accNum);
    }

    @Override
    public void displayDetails() {
        System.out.println("\n----- ACCOUNT DETAILS -----");
        System.out.println("Account Number: " + accountNumber);
        System.out.println("Holder Name: " + accountHolderName);
        System.out.println("Branch: " + branchName);
        System.out.println("Balance: " + balance);

        System.out.println("Recent Transactions:");
        for (double amt : recentTransactions) {
            System.out.println(amt);
        }
    }

    public String getAccountNumber() { return accountNumber; }
    public String getBranchName() { return branchName; }
    public double getBalance() { return balance; }
}

// ===== SAVINGS ACCOUNT =====
class SavingsAccount extends Account {

    private double interestRate;
    private int withdrawalLimit;
    private int withdrawalCount;

    public SavingsAccount(String accNum, String holder, String branch, double bal,
                          double interestRate, int limit) {
        super(accNum, holder, branch, bal);
        this.interestRate = interestRate;
        this.withdrawalLimit = limit;
        this.withdrawalCount = 0;
    }

    @Override
    public double calculateInterestOrCharges() {
        return (balance * interestRate) / 100;
    }

    @Override
    public void withdraw(double amount) {
        if (withdrawalCount >= withdrawalLimit) {
            System.out.println("❌ Withdrawal limit reached!");
            return;
        }

        if (amount > balance) {
            System.out.println("❌ Insufficient balance!");
            return;
        }

        super.withdraw(amount);
        withdrawalCount++;
    }
}

// ===== CURRENT ACCOUNT =====
class CurrentAccount extends Account {

    private double overdraftLimit;
    private double maintenanceCharge;

    public CurrentAccount(String accNum, String holder, String branch, double bal,
                          double overdraft, double charge) {
        super(accNum, holder, branch, bal);
        this.overdraftLimit = overdraft;
        this.maintenanceCharge = charge;
    }

    @Override
    public double calculateInterestOrCharges() {
        return -maintenanceCharge;
    }

    @Override
    public void withdraw(double amount) {
        if (balance - amount < -overdraftLimit) {
            System.out.println("❌ Overdraft limit exceeded!");
            return;
        }
        super.withdraw(amount);
    }
}

// ===== MAIN CLASS =====
public class BankingSystem {

    private ArrayList<Account> accounts = new ArrayList<>();
    private HashSet<String> branches = new HashSet<>();
    Scanner sc = new Scanner(System.in);

    public void createAccount() {
        System.out.print("Enter Account Number: ");
        String accNum = sc.next();

        System.out.print("Enter Holder Name: ");
        String holder = sc.next();

        System.out.print("Enter Branch Name: ");
        String branch = sc.next();
        branches.add(branch);

        System.out.print("Initial Balance: ");
        double bal = sc.nextDouble();

        System.out.println("1. Savings Account");
        System.out.println("2. Current Account");
        System.out.print("Choose type: ");
        int type = sc.nextInt();

        if (type == 1) {
            System.out.print("Interest Rate: ");
            double rate = sc.nextDouble();
            System.out.print("Withdrawal Limit: ");
            int limit = sc.nextInt();

            accounts.add(new SavingsAccount(accNum, holder, branch, bal, rate, limit));
        } 
        else {
            System.out.print("Overdraft Limit: ");
            double overdraft = sc.nextDouble();
            System.out.print("Maintenance Charge: ");
            double charge = sc.nextDouble();

            accounts.add(new CurrentAccount(accNum, holder, branch, bal, overdraft, charge));
        }

        System.out.println("✔ Account Created Successfully!");
    }

    public Account searchAccountByNumber(String accNum) throws AccountNotFoundException {
        for (Account a : accounts) {
            if (a.matchesAccountNumber(accNum)) {
                return a;
            }
        }
        throw new AccountNotFoundException("❌ Account Not Found!");
    }

    public void processDeposit() throws Exception {
        System.out.print("Enter Account Number: ");
        String acc = sc.next();

        Account a = searchAccountByNumber(acc);

        System.out.print("Enter Amount: ");
        double amt = sc.nextDouble();

        a.deposit(amt);
        System.out.println("✔ Deposit Successful!");
    }

    public void processWithdrawal() throws Exception {
        System.out.print("Enter Account Number: ");
        String acc = sc.next();

        Account a = searchAccountByNumber(acc);

        System.out.print("Enter Amount: ");
        double amt = sc.nextDouble();

        a.withdraw(amt);
    }

    public void displayAllAccounts() {
        for (Account a : accounts) {
            a.displayDetails();
        }

        double total = 0;
        for (Account a : accounts) {
            total += a.getBalance();
        }

        System.out.println("\nTotal Deposits in Bank: " + total);
        System.out.println("Unique Branches: " + branches);
    }

    public void showMenu() throws Exception {
        while (true) {
            System.out.println("\n===== BANKING MENU =====");
            System.out.println("1. Create Account");
            System.out.println("2. Deposit Money");
            System.out.println("3. Withdraw Money");
            System.out.println("4. Display All Accounts");
            System.out.println("5. Exit");

            System.out.print("Enter Choice: ");
            int ch = sc.nextInt();

            switch (ch) {
                case 1: createAccount(); break;
                case 2: processDeposit(); break;
                case 3: processWithdrawal(); break;
                case 4: displayAllAccounts(); break;
                case 5: return;
                default: System.out.println("❌ Invalid Choice!");
            }
        }
    }

    public static void main(String[] args) throws Exception {
        new BankingSystem().showMenu();
    }
}
