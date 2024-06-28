const notificationsection = document.querySelector('.notificationsection')
// const notificationtext = document.querySelector('.notificationtext')

const notification = (text, type) => {
    const notificationbox = document.createElement('div')
    const notificationtextbox = document.createElement('div')
    const notificationtext = document.createElement('h3')
    notificationtext.classList.add('notificationtext')
    notificationtextbox.append(notificationtext)
    notificationtextbox.classList.add('notificationtextbox')
    notificationbox.append(notificationtextbox)
    notificationbox.classList.add('notification')
    notificationbox.addEventListener('click', () => {
        notificationbox.classList.remove('active')
    })
    notificationsection.append(notificationbox)
    if (type === "error") {
        notificationbox.classList.remove('success')
        notificationbox.classList.add('error')
    } else {
        notificationbox.classList.remove('error')
        notificationbox.classList.add('success')
    }
    notificationtext.innerHTML = text
    setTimeout(() => {
        notificationbox.classList.add('active')
    }, 300)
    setTimeout(() => {
        notificationbox.classList.remove('active')
        setTimeout(() => {
            notificationsection.removeChild(notificationbox)
        }, 200)
    }, 3000)
}

// const closenotification = () => {
    // notificationbox.classList.remove('active')
// }

// notificationbox.addEventListener('click', closenotification)