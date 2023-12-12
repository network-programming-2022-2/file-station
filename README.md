# FileStation - Peer-to-Peer File Sharing Application

## Overview

FileStation is a C-based file-sharing application with a peer-to-peer model and an additional client-server component to help users keep track of all shared files. 

This README outlines the key features, technologies used in the creation of FileStation, and quick start instructions for running it locally.

## Table of Contents

- [Features](#features)
- [Technologies Used](#technologies-used)
- [Quick Start](#quick-start)

## Features

1. **Authentication:**
   - Users can register, login, and manage their accounts.

2. **Automatically keeping track with the shared files:**
   - Upload/Update/Remove files automatically from the server database if there are new files in the directory, or files got changed/deleted. There is also option to manage manually.

3. **Search files:**
   - Users can find available files from the server, and their owners' information to ask for transferring the files.

4. **Download files from other clients:**
   - User can download a file by providing file owner information, and the file owner will send the file.
   
## Technologies Used

- C
- GTK
- PostgreSQL

## Quick Start

To get started with FileStation, follow these steps:

1. **Clone the Repository:**
   ```bash
   git clone https://github.com/network-programming-2022-2/file-station.git
   cd file-station
   ```
   
2. **Run the Server:**
   - Open the server in your preferred C development environment.
   - Provide the IP and the port.
   - Build and run the server.
   
3. **Run the Client Interface:**
   - Open the client in another window.
   - Provide the IP and port of the server, IP and port of the client, and path to the shared directory.
   - Build and run the client.

3. **Try the application:**
   - Create a new account and log in
   - Use the search functionality to find files
   - Use the download functionality to download files
