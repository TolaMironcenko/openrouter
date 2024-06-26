const authhost = 'http://192.168.122.76:43243'
const host = 'http://192.168.122.76:43244'
const prefix = 'api'

const routes = {
    login: () => [authhost, prefix, 'token'].join('/'),
    user: () => [authhost, prefix, 'users', 'user'].join('/'),
    telnet_status: () => [host, prefix, 'settings', 'telnet', 'get'].join('/'),
    telnet_status_set: () => [host, prefix, 'settings', 'telnet', 'set'].join('/'),
    reboot: () => [host, prefix, 'reboot'].join('/')
    // all_users: () => [host, prefix, 'users', 'all'].join('/'),
    // access: () => [host, prefix, 'access'].join('/'),
    // adduser: () => [host, prefix, 'users', 'add'].join('/'),
    // deluser: () => [host, prefix, 'users', 'del'].join('/'),
    // change_password_url: () => [host, prefix, 'users', 'user', 'password', 'change'].join('/'),
    // change_user: () => [host, prefix, 'users', 'user', 'change'].join('/')
}