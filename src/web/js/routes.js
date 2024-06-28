const authhost = 'http://172.20.10.2:43243'
const host = window.location.origin
const prefix = 'api'

const routes = {
    login: () => [authhost, prefix, 'token'].join('/'),
    user: () => [authhost, prefix, 'users', 'user'].join('/'),
    telnet_status: () => [host, prefix, 'settings', 'telnet', 'get'].join('/'),
    telnet_status_set: () => [host, prefix, 'settings', 'telnet', 'set'].join('/'),

    dns_status: () => [host, prefix, 'settings', 'dns', 'get'].join('/'),
    dns_status_set: () => [host, prefix, 'settings', 'dns', 'set'].join('/'),
    dns_servers_get: () => [host, prefix, 'settings', 'dns', 'servers', 'get'].join('/'),
    dns_servers_set: () => [host, prefix, 'settings', 'dns', 'servers', 'set'].join('/'),

    ftp_status: () => [host, prefix, 'settings', 'ftp', 'get'].join('/'),
    ftp_status_set: () => [host, prefix, 'settings', 'ftp', 'set'].join('/'),

    reboot: () => [host, prefix, 'reboot'].join('/')
    // all_users: () => [host, prefix, 'users', 'all'].join('/'),
    // access: () => [host, prefix, 'access'].join('/'),
    // adduser: () => [host, prefix, 'users', 'add'].join('/'),
    // deluser: () => [host, prefix, 'users', 'del'].join('/'),
    // change_password_url: () => [host, prefix, 'users', 'user', 'password', 'change'].join('/'),
    // change_user: () => [host, prefix, 'users', 'user', 'change'].join('/')
}