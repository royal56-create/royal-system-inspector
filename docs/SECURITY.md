# Security and privacy

Royal System Inspector is intended for transparent, local, read-only inspection.

- It reads operating-system information through standard OS APIs and read-only system interfaces.
- It does not request administrator/root privileges.
- It does not transmit information, run a background service, or collect telemetry.
- It does not modify settings, delete files, terminate processes, capture network traffic, or collect credentials.
- The process command reports a count only; it does not expose process command lines or terminate processes.
- The report command writes the requested report file in the current working directory. Review the destination and contents before sharing a report, because host and system metadata may be personal information.

Report suspected vulnerabilities privately to the repository owner rather than publishing exploit details in an issue.
