#include <QDebug>
#include "data/logsdata.hh"
int main(int argc, char **argv) {
    QString path = (argc > 1) ? QString::fromLocal8Bit(argv[1])
                              : QString("/caminho/para/seu/arquivo.log.gz");
    LogsData logSource(path);
    if (!logSource.open()) {
        qCritical() << "Falha ao abrir o arquivo de log:" << path;
        return 1;
    }
    qInfo() << "Log aberto com sucesso! Lendo snapshots...";
    int frameCount = 0;
    while (logSource.hasNext()) {
        WorldSnapshot snap = logSource.nextFrame();
        frameCount++;
        // Imprime os 5 primeiros frames e depois a cada 500 frames para conferência
        if (frameCount <= 5 || frameCount % 500 == 0) {
            qInfo() << "Frame #" << frameCount
                    << "| t =" << QString::number(snap.timestamp(), 'f', 3)
                    << "| Bola:" << vec2ToString(snap.ball().pos())
                    << "| Robôs:" << snap.robots().size()
                    << "| RefCmd:" << snap.refereeCommand();
        }
    }
    qInfo() << "Leitura concluída! Total de snapshots processados:" << frameCount;
    return 0;
}