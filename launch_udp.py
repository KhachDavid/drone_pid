import subprocess
import paramiko

# Step 1: Run local script
try:
    print("Running local script: udp_tx.py")
    subprocess.run(["python3", "udp_tx.py"], check=True)
except subprocess.CalledProcessError as e:
    print(f"Failed to run udp_tx.py: {e}")
    exit(1)

# Step 2: SSH and run remote command
hostname = "10.42.0.30"
username = "pi"
remote_command = "cd /home/david/FlightController/week2 && ./udp_tx"

try:
    print(f"Connecting to {username}@{hostname}")
    ssh = paramiko.SSHClient()
    ssh.set_missing_host_key_policy(paramiko.AutoAddPolicy())
    ssh.connect(hostname, username=username)

    print(f"Running remote command: {remote_command}")
    stdin, stdout, stderr = ssh.exec_command(remote_command)

    print("Output:")
    for line in stdout:
        print(line.strip())

    print("Errors (if any):")
    for line in stderr:
        print(line.strip())

    ssh.close()
except Exception as e:
    print(f"SSH connection or command failed: {e}")
    exit(1)

