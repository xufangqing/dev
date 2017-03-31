#!/bin/bash

CNAME=nfoam

/usr/bin/expect <<-EOF
	set timeout 5     
	spawn docker exec -it ${CNAME} /bin/bash
	expect  {
		"nfoam /" {
			puts "111111111111111111111"
			set raw $expect_out(buffer)
		}
		timeout {
			send_tty "\n\[error\]: timeout for docker exec nfoam\n"
			exit 254
		}
		eof {
			send_tty "\n\[error\]: eof for docker exec nfoam\n"
			exit 253
		}
	}
	
	send "/bin/sysrepoctl.sh -l\n"
	expect  {
		"nfoam /" {
			puts $expect_out(buffer);
			puts "tttttttttttttttttt"
			set raw $expect_out(buffer)
		}
		timeout {
			puts "\n\[error\]: timeout for docker exec nfoam\n"
			exit 254
		}
		eof {
			puts "\n\[error\]: eof for docker exec nfoam\n"
			exit 253
		}
	}
	
	set ret [regexp {^TransportNetwork.*} $raw match]
	puts "$raw"
#	puts "$match"
	puts "$ret"
	puts "22222222222222222222222222"
	if { $ret == "1" } {
		puts "model TransportNetwork not upload";
	} else {
		puts "model TransportNetwork uploaded"
	}
#	ManagedElementData
#	Equipment 

	send "exit\n";
	
	interact
	
EOF
