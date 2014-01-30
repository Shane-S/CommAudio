#include "ClientTransfer.h"

/*
 * Post-condition:
 * If this returns true, the socket descriptor in props is (or was at one point) ready to send on.
 * If this returns false, the socket descriptor in props will be 0.
 */
BOOL ClientInitSocket(LPTransferProps props)
{
	if (props->szHostName[0] != 0) // They specified a host name
	{
		struct hostent *hp;
		char		   name[HOSTNAME_SIZE];

		TCHAR_2_CHAR(name, props->szHostName, HOSTNAME_SIZE);
		hp = gethostbyname(name);

		if (hp == NULL)
		{
			switch (h_errno)
			{
			case HOST_NOT_FOUND:
				MessageBoxPrintf(MB_ICONERROR, TEXT("Host Not Found"),
					TEXT("Could not find host %s. Check connection settings."), props->szHostName);
				return FALSE;
			case TRY_AGAIN:
				MessageBoxPrintf(MB_ICONERROR, TEXT("Try Again"), TEXT("Unable to connect. Try again later."));
				return FALSE;
			case NO_RECOVERY:
				MessageBoxPrintf(MB_ICONERROR, TEXT("DNS Error"), TEXT("DNS error. Try again later."));
				return FALSE;
			case NO_ADDRESS:
				MessageBoxPrintf(MB_ICONERROR, TEXT("No IP"), TEXT("No IP address for host %s. Check connection settings."),
					props->szHostName);
				return FALSE;
			default:
				MessageBoxPrintf(MB_ICONERROR, TEXT("Unknown Error"), TEXT("Unkown error %d."), h_errno);
				return FALSE;
			}
		}
		props->paddr_in->sin_addr.s_addr = (ULONG)hp->h_addr_list[0];
	}
	
	if (props->paddr_in->sin_addr.s_addr != 0)
	{
		SOCKET s;
		if ((s = socket(PF_INET, props->nSockType, 0)) < 0)
		{
			MessageBox(NULL, TEXT("Could not create socket."), TEXT("No Socket"), MB_ICONERROR);
			return FALSE;
		}

		if (props->nSockType == SOCK_DGRAM)
		{
			props->socket = s;
			return TRUE;
		}

		if (connect(s, (sockaddr *)props->paddr_in, sizeof(sockaddr)) == -1)
		{
			MessageBox(NULL, TEXT("Could not connect to socket; "),
				TEXT("Could not connect. Check settings and try again."), MB_ICONERROR);
			return FALSE;
		}
		props->socket = s;
		return TRUE;
	}
	else
	{
		MessageBox(NULL, TEXT("No IP address or host name entered. Check connection settings."), TEXT("No Destination"),
			MB_ICONERROR);
		return FALSE;
	}
}
