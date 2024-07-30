const host = window.location.origin
const prefix = 'api'

const routes = {
    login: () => [host, prefix, 'token'].join('/'),
    user: () => [host, prefix, 'users', 'user'].join('/'),

    telnet_status: () => [host, prefix, 'settings', 'telnet', 'get'].join('/'),
    telnet_status_set: () => [host, prefix, 'settings', 'telnet', 'set'].join('/'),
    telnet_port_get: () => [host, prefix, 'settings', 'telnet', 'port', 'get'].join('/'),
    telnet_port_set: () => [host, prefix, 'settings', 'telnet', 'port', 'set'].join('/'),

    ssh_status_get: () => [host, prefix, 'settings', 'ssh', 'status', 'get'].join('/'),
    ssh_status_set: () => [host, prefix, 'settings', 'ssh', 'status', 'set'].join('/'),
    ssh_keys_get: () => [host, prefix, 'settings', 'ssh', 'keys', 'get'].join('/'),
    ssh_keys_set: () => [host, prefix, 'settings', 'ssh', 'keys', 'set'].join('/'),
    ssh_port_get: () => [host, prefix, 'settings', 'ssh', 'port', 'get'].join('/'),
    ssh_port_set: () => [host, prefix, 'settings', 'ssh', 'port', 'set'].join('/'),

    dns_status: () => [host, prefix, 'settings', 'dns', 'get'].join('/'),
    dns_status_set: () => [host, prefix, 'settings', 'dns', 'set'].join('/'),
    dns_servers_get: () => [host, prefix, 'settings', 'dns', 'servers', 'get'].join('/'),
    dns_servers_set: () => [host, prefix, 'settings', 'dns', 'servers', 'set'].join('/'),
    dns_local_domains_get: () => [host, prefix, 'settings', 'dns', 'domains', 'get'].join('/'),
    dns_local_domains_set: () => [host, prefix, 'settings', 'dns', 'domains', 'set'].join('/'),

    ftp_status: () => [host, prefix, 'settings', 'ftp', 'get'].join('/'),
    ftp_status_set: () => [host, prefix, 'settings', 'ftp', 'set'].join('/'),

    wifi_status_get: () => [host, prefix, 'settings', 'wifi', 'status', 'get'].join('/'),
    wifi_status_set: () => [host, prefix, 'settings', 'wifi', 'status', 'set'].join('/'),
    wifi_name_get: () => [host, prefix, 'settings', 'wifi', 'name', 'get'].join('/'),
    wifi_name_set: () => [host, prefix, 'settings', 'wifi', 'name', 'set'].join('/'),
    wifi_password_get: () => [host, prefix, 'settings', 'wifi', 'password', 'get'].join('/'),
    wifi_password_set: () => [host, prefix, 'settings', 'wifi', 'password', 'set'].join('/'),
    wifi_qr_get: () => [host, prefix, 'settings', 'wifi', 'qr', 'get'].join('/'),
    wifi_channel_get: () => [host, prefix, 'settings', 'wifi', 'channel', 'get'].join('/'),
    wifi_channel_set: () => [host, prefix, 'settings', 'wifi', 'channel', 'set'].join('/'),

    reboot: () => [host, prefix, 'reboot'].join('/'),
    syslog: () => [host, prefix, 'syslog'].join('/'),
    uptime: () => [host, prefix, 'uptime'].join('/'),
}