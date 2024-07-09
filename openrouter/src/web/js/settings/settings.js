const get_all_settings = () => {
    get_dns_enabled_status()
    get_dns_server()
    get_local_domains()

    get_ftp_enabled_status()

    get_telnet_enabled_status()
    get_telnet_port()

    get_ssh_status()
    get_ssh_keys()
    get_ssh_port()

    get_wifi_status()
    get_wifi_name()
    get_wifi_password()
}
