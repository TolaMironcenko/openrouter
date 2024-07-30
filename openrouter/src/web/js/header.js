const settingssection = document.querySelector('.settingssection')
const settingsbutton = document.querySelector('#settingsbutton')
const syslogsection = document.querySelector('.syslogsection')
const syslogbutton = document.querySelector('#syslogbutton')

const headermenubutton = document.querySelector('#headermenubutton')
const headerlinks = document.querySelector('.links')
// const header = document.querySelector('.header')

settingsbutton.addEventListener('click', () => {
    settingssection.classList.add('active')
    syslogsection.classList.remove('active')
})

syslogbutton.addEventListener('click', () => {
    syslogsection.classList.add('active')
    settingssection.classList.remove('active')
    get_syslog_data()
})

headermenubutton.addEventListener('click', () => {
    if (headerlinks.classList.contains('active')) {
        headerlinks.classList.remove('active')
        return
    }
    headerlinks.classList.add('active')
})

window.addEventListener('click', (e) => {
    if (e.target.id !== "headermenubutton" && !e.target.classList.contains('links')) {
        headerlinks.classList.remove('active')
    }
})

// if (window.screenX > 600) {
//     header.removeChild(headermenubutton)
// } else {
//     header.prepend(headermenubutton)
// }
