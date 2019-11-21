pipeline {
    agent {
		label 'Unreal4212'
	}
	options {
		buildDiscarder logRotator(artifactDaysToKeepStr: '', artifactNumToKeepStr: '', daysToKeepStr: '', numToKeepStr: '10')
		disableConcurrentBuilds()
	}
	parameters {
		credentials credentialType: 'org.jenkinsci.plugins.plaincredentials.impl.StringCredentialsImpl', defaultValue: '4cc13aea-ea4b-477c-9bc8-f54252d6fcc0', description: '', name: 'WebhookUrl', required: false
	}
	
    stages{
        stage('scm'){
			steps{
				checkout scm
			}
		}
		stage('Build: Prep'){
			steps{
				bat label: 'Prepare', script: '"%Unreal 4.21.2_HOME%\\%Unreal_BuildTool%" -projectfiles -project="%WORKSPACE%\\Casino_Inc\\Casino_Inc.uproject" -game -rocket -progress'
			}
		}
		stage('Build: Build'){
			steps{
				bat label: 'Build', script: '"%Unreal 4.21.2_HOME%\\%Unreal_RunUAT%" BuildCookRun -project="%WORKSPACE%\\Casino_Inc\\Casino_Inc.uproject" -noP4 -platform=Win64 -clientconfig=Development -cook -allmaps -build -stage -pak -archive -archivedirectory="%WORKSPACE%\\Builds"'
			}
		}
		stage('Build: Cook'){
			steps{
				bat label: 'cook', script: '"%Unreal 4.21.2_HOME%\\%Unreal_RunUAT%" BuildCookRun -project="%WORKSPACE%\\Casino_Inc\\Casino_Inc.uproject" -noP4 -platform=Win64 -clientconfig=Development -cook -allmaps -NoCompile -stage -pak -archive -archivedirectory="%WORKSPACE%\\Builds"'
			}
		}
    }
	post {
		always {
			archiveArtifacts 'Builds/**/*.*'
			
			sendDiscord()
		}
	}
}

@NonCPS
def sendDiscord(){
	def changeString = ""
	
	def changeSets = currentBuild.changeSets
	for (int i = 0; i < changeSets.size(); i++){
		def entries = changeSets[i].items
		for (int j = 0; j < entries.length; j++){
			def entry = entries[j]
			truncated_ID = entry.commitId.take(7)
			truncated_msg = entry.msg.take(100)
			changeString += "\n- `${truncated_ID}` *${truncated_msg} -  ${entry.author}*"
		}
	}
	
	if (!changeString) {
        changeString = "\n\n - No new changes"
    }
    
	withCredentials([string(credentialsId: params.WebhookUrl, variable: 'URL')]) {
		def currentWebhookUrl = URL
	}
	
	discordSend description: "**Build:** ${env.BUILD_NUMBER}\n**Status:** ${currentBuild.currentResult}\n\n**Changes:**${changeString}\n\n**Artifacts:**\n- ${env.BUILD_URL}artifact/",
	footer: '',
	image: '',
	link: env.BUILD_URL,
	result: currentBuild.currentResult,
	thumbnail: '',
	title: env.BRANCH_NAME,
	webhookURL: currentWebhookUrl
}