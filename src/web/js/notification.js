const notificationbox = document.querySelector('.notification')
const notificationtext = document.querySelector('.notificationtext')

const notification = (text, type) => {
    if (type === "error") {
        notificationbox.classList.remove('success')
        notificationbox.classList.add('error')
    } else {
        notificationbox.classList.remove('error')
        notificationbox.classList.add('success')
    }
    notificationtext.innerHTML = text
    notificationbox.classList.add('active')
    setTimeout(() => {
        notificationbox.classList.remove('active')
    }, 2000)
}

const closenotification = () => {
    notificationbox.classList.remove('active')
}

notificationbox.addEventListener('click', closenotification)